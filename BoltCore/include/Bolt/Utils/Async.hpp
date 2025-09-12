#pragma once
#include "Bolt/Core/Time.hpp"
#include "Bolt/Debug/Logger.hpp"

#include <thread>
#include <chrono>
#include <utility>
#include <iostream>
#include <future>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <vector>
#include <type_traits>


namespace Bolt {
    inline void WaitForSeconds(double seconds) {
        double timeScale = Time::GetTimeScale();
        if (timeScale <= 0.0001) {
            while (Time::GetTimeScale() <= 0.0001) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            timeScale = Time::GetTimeScale();
        }
        double scaledSeconds = seconds / timeScale;
        std::this_thread::sleep_for(std::chrono::duration<double>(scaledSeconds));
    }

    inline void WaitForSecondsRealtime(double seconds) {
        std::this_thread::sleep_for(std::chrono::duration<double>(seconds));
    }

    inline void WaitForMillisecondsRealtime(int milliseconds) {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    }

    inline void WaitForMilliseconds(int milliseconds) {
        double timeScale = Time::GetTimeScale();
        if (timeScale <= 0.0001) {
            while (Time::GetTimeScale() <= 0.0001) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            timeScale = Time::GetTimeScale();
        }
        double scaledMs = milliseconds / timeScale;
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(scaledMs + 0.5)));
    }

    // Warte auf Condition
    template<typename Predicate>
    inline void WaitUntil(Predicate&& pred, int checkIntervalMs = 10) {
        while (!pred()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(checkIntervalMs));
        }
    }

    // Warte auf Condition mit Timeout
    template<typename Predicate>
    inline bool WaitUntilTimeout(Predicate&& pred, double timeoutSeconds, int checkIntervalMs = 10) {
        auto start = std::chrono::steady_clock::now();
        auto timeout = std::chrono::duration<double>(timeoutSeconds);

        while (!pred()) {
            if (std::chrono::steady_clock::now() - start > timeout) {
                return false; // Timeout
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(checkIntervalMs));
        }
        return true; // Condition erfüllt
    }

    // ============================================
    // Thread Pool für effizientere Async-Operationen
    // ============================================
    class ThreadPool {
    public:
        static ThreadPool& GetInstance() {
            static ThreadPool instance;
            return instance;
        }

        template<typename Func, typename... Args>
        auto Enqueue(Func&& func, Args&&... args)
            -> std::future<typename std::invoke_result<Func, Args...>::type> {
            using return_type = typename std::invoke_result<Func, Args...>::type;

            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<Func>(func), std::forward<Args>(args)...)
            );

            std::future<return_type> result = task->get_future();
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);

                if (m_Stop) {
                   throw std::runtime_error("ThreadPool has been stopped");
                }

                m_Tasks.emplace([task]() { (*task)(); });
            }
            m_Condition.notify_one();
            return result;
        }

        void SetThreadCount(size_t count) {
            // Kann nur vor dem ersten Gebrauch gesetzt werden
            if (m_Workers.empty() && count > 0) {
                m_ThreadCount = count;
                Initialize();
            }
        }

        size_t GetThreadCount() const { return m_ThreadCount; }

        ~ThreadPool() {
            {
                std::unique_lock<std::mutex> lock(m_QueueMutex);
                m_Stop = true;
            }
            m_Condition.notify_all();
            for (std::thread& worker : m_Workers) {
                if (worker.joinable()) {
                    worker.join();
                }
            }
        }

    private:
        ThreadPool() : m_ThreadCount(std::thread::hardware_concurrency()) {
            if (m_ThreadCount == 0) m_ThreadCount = 4; // Fallback
            Initialize();
        }

        void Initialize() {
            for (size_t i = 0; i < m_ThreadCount; ++i) {
                m_Workers.emplace_back([this] {
                    for (;;) {
                        std::function<void()> task;
                        {
                            std::unique_lock<std::mutex> lock(this->m_QueueMutex);
                            this->m_Condition.wait(lock, [this] {
                                return this->m_Stop || !this->m_Tasks.empty();
                                });
                            if (this->m_Stop && this->m_Tasks.empty()) {
                                return;
                            }
                            task = std::move(this->m_Tasks.front());
                            this->m_Tasks.pop();
                        }
                        task();
                    }
                    });
            }
        }

        std::vector<std::thread> m_Workers;
        std::queue<std::function<void()>> m_Tasks;
        std::mutex m_QueueMutex;
        std::condition_variable m_Condition;
        bool m_Stop = false;
        size_t m_ThreadCount;
    };

    // ============================================
    // Async Task Handle
    // ============================================
    template<typename T>
    class AsyncTask {
    public:
        AsyncTask(std::future<T>&& future)
            : m_Future(std::move(future)) {
        }

        // Prüft ob Task fertig ist
        bool IsReady() const {
            return m_Future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }

        // Wartet auf Ergebnis
        T Get() {
            return m_Future.get();
        }

        // Wartet mit Timeout
        bool Wait(double seconds) {
            return m_Future.wait_for(std::chrono::duration<double>(seconds))
                == std::future_status::ready;
        }

        // Wartet und gibt Status zurück
        std::future_status WaitFor(double seconds) {
            return m_Future.wait_for(std::chrono::duration<double>(seconds));
        }

    private:
        std::future<T> m_Future;
    };

    // Spezialisierung für void
    template<>
    class AsyncTask<void> {
    public:
        AsyncTask(std::future<void>&& future)
            : m_Future(std::move(future)) {
        }

        bool IsReady() const {
            return m_Future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        }

        void Get() {
            m_Future.get();
        }

        bool Wait(double seconds) {
            return m_Future.wait_for(std::chrono::duration<double>(seconds))
                == std::future_status::ready;
        }

        std::future_status WaitFor(double seconds) {
            return m_Future.wait_for(std::chrono::duration<double>(seconds));
        }

    private:
        std::future<void> m_Future;
    };

    // ============================================
    // Async Functions
    // ============================================

    // Fire-and-forget async (detached thread)
    template<typename Func>
    inline void Start(Func&& func) {
        std::thread([f = std::forward<Func>(func)]() mutable {
            try {
                f();
            }
            catch (const std::exception& ex) {
                std::string errorMsg = ex.what();
                Logger::Error ("[Async] Unhandled exception: " + errorMsg);
            }
            catch (...) {
                Logger::Error("[Async] Unhandled unknown exception");
            }
            }).detach();
    }

    // Async mit Rückgabewert (verwendet ThreadPool)
    template<typename Func, typename... Args>
    inline auto Async(Func&& func, Args&&... args)
        -> AsyncTask<typename std::invoke_result<Func, Args...>::type> {
        auto future = ThreadPool::GetInstance().Enqueue(
            std::forward<Func>(func),
            std::forward<Args>(args)...
        );
        return AsyncTask<typename std::invoke_result<Func, Args...>::type>(std::move(future));
    }

    // Async mit std::async (für einfache Fälle ohne ThreadPool)
    template<typename Func, typename... Args>
    inline auto AsyncDirect(Func&& func, Args&&... args)
        -> AsyncTask<typename std::invoke_result<Func, Args...>::type> {
        auto future = std::async(
            std::launch::async,
            std::forward<Func>(func),
            std::forward<Args>(args)...
        );
        return AsyncTask<typename std::invoke_result<Func, Args...>::type>(std::move(future));
    }

    // Verzögerter Start
    template<typename Func>
    inline void StartDelayed(double seconds, Func&& func) {
        Start([seconds, f = std::forward<Func>(func)]() mutable {
            WaitForSecondsRealtime(seconds);
            f();
            });
    }

    // Verzögerter Start mit Game Time
    template<typename Func>
    inline void StartDelayedGameTime(double seconds, Func&& func) {
        Start([seconds, f = std::forward<Func>(func)]() mutable {
            WaitForSeconds(seconds);
            f();
            });
    }

    // Wiederholte Ausführung
    template<typename Func>
    inline std::shared_ptr<std::atomic<bool>> StartRepeating(
        double intervalSeconds,
        Func&& func,
        bool useGameTime = false) {

        auto running = std::make_shared<std::atomic<bool>>(true);

        Start([running, intervalSeconds, f = std::forward<Func>(func), useGameTime]() mutable {
            while (*running) {
                f();
                if (useGameTime) {
                    WaitForSeconds(intervalSeconds);
                }
                else {
                    WaitForSecondsRealtime(intervalSeconds);
                }
            }
            });

        return running; // Kann auf false gesetzt werden um zu stoppen
    }

    // ============================================
    // Parallel Execution
    // ============================================

    // Führt mehrere Tasks parallel aus und wartet auf alle
    template<typename... Funcs>
    inline void Parallel(Funcs&&... funcs) {
        std::vector<std::thread> threads;
        threads.reserve(sizeof...(funcs));

        // Helper um alle Funktionen zu starten
        auto startFunc = [&threads](auto&& func) {
            threads.emplace_back(std::forward<decltype(func)>(func));
            };

        (startFunc(std::forward<Funcs>(funcs)), ...);

        // Warte auf alle Threads
        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }

    // Führt Funktion auf einer Collection parallel aus
    template<typename Iterator, typename Func>
    inline void ParallelFor(Iterator begin, Iterator end, Func&& func) {
        size_t count = std::distance(begin, end);
        size_t threadCount = std::min(
            static_cast<size_t>(std::thread::hardware_concurrency()),
            count
        );

        if (threadCount <= 1) {
            // Sequenziell ausführen
            for (auto it = begin; it != end; ++it) {
                func(*it);
            }
            return;
        }

        std::vector<std::thread> threads;
        threads.reserve(threadCount);

        size_t chunkSize = count / threadCount;
        size_t remainder = count % threadCount;

        auto current = begin;
        for (size_t i = 0; i < threadCount; ++i) {
            size_t thisChunkSize = chunkSize + (i < remainder ? 1 : 0);
            auto chunkEnd = current;
            std::advance(chunkEnd, thisChunkSize);

            threads.emplace_back([current, chunkEnd, &func]() {
                for (auto it = current; it != chunkEnd; ++it) {
                    func(*it);
                }
                });

            current = chunkEnd;
        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
    }
}

// ============================================
// Macros für einfache Verwendung
// ============================================

// Fire-and-forget
#define ASYNC(...) Bolt::Start([=]() { __VA_ARGS__; })
#define ASYNC_REF(...) Bolt::Start([&]() { __VA_ARGS__; })
#define ASYNC_MOVE(...) Bolt::Start([=]() mutable { __VA_ARGS__; })

// Mit Verzögerung
#define ASYNC_DELAYED(seconds, ...) Bolt::StartDelayed(seconds, [=]() { __VA_ARGS__; })
#define ASYNC_DELAYED_GAMETIME(seconds, ...) Bolt::StartDelayedGameTime(seconds, [=]() { __VA_ARGS__; })

// Wiederholend
#define ASYNC_REPEATING(interval, handle, ...) \
    auto handle = Bolt::StartRepeating(interval, [=]() { __VA_ARGS__; })

// Parallel
#define PARALLEL(...) Bolt::Parallel(__VA_ARGS__)