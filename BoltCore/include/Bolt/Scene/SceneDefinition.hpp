#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <typeindex>

namespace Bolt {
    class Scene;
    class ISystem;

    /**
     * SceneDefinition enthält die "Bauanleitung" für eine Scene.
     * Sie speichert, welche Systems erstellt werden sollen und andere
     * persistente Konfigurationen, aber keine Runtime-Daten.
     */
    class SceneDefinition {
        friend class SceneManager;

    public:
        explicit SceneDefinition(const std::string& name)
            : m_name(name) {
        }

        /**
         * Fügt ein System zur Scene-Definition hinzu.
         * Die Factory-Funktion wird gespeichert und beim Laden der Scene verwendet.
         */
        template<typename TSystem, typename... Args>
        SceneDefinition& addSystem(Args&&... args) {
            static_assert(std::is_base_of<ISystem, TSystem>::value,
                "TSystem must derive from ISystem");

            // Speichere Factory-Funktion mit den Arguments
            m_systemFactories.emplace_back(
                [capturedArgs = std::tuple<Args...>(std::forward<Args>(args)...)]() mutable {
                    return std::apply(
                        [](auto&&... a) {
                            return std::make_unique<TSystem>(std::forward<decltype(a)>(a)...);
                        },
                        capturedArgs
                    );
                }
            );

            // Speichere Typ-Info für spätere Queries
            m_systemTypes.push_back(std::type_index(typeid(TSystem)));

            return *this;
        }

        /**
         * Setzt eine Callback-Funktion, die beim Initialisieren der Scene aufgerufen wird.
         * Nützlich für Scene-spezifische Setups wie Kamera-Position, Skybox, etc.
         */
        SceneDefinition& onInitialize(std::function<void(Scene&)> callback) {
            m_initializeCallbacks.push_back(callback);
            return *this;
        }

        /**
         * Setzt eine Callback-Funktion, die beim Laden der Scene aufgerufen wird.
         * Wird nach der Initialisierung aber vor Start der Systems aufgerufen.
         */
        SceneDefinition& onLoad(std::function<void(Scene&)> callback) {
            m_loadCallbacks.push_back(callback);
            return *this;
        }

        /**
         * Setzt eine Callback-Funktion, die beim Entladen der Scene aufgerufen wird.
         */
        SceneDefinition& onUnload(std::function<void(Scene&)> callback) {
            m_unloadCallbacks.push_back(callback);
            return *this;
        }

        /**
         * Markiert diese Scene als Startup-Scene (wird automatisch beim Start geladen)
         */
        SceneDefinition& setAsStartupScene() {
            m_isStartupScene = true;
            return *this;
        }

        /**
         * Setzt Scene-spezifische Eigenschaften
         */
        SceneDefinition& setPersistent(bool persistent) {
            m_isPersistent = persistent;
            return *this;
        }

        const std::string& getName() const { return m_name; }
        bool isStartupScene() const { return m_isStartupScene; }
        bool isPersistent() const { return m_isPersistent; }

    private:
        friend class Scene;
        friend class SceneManager;

        std::string m_name;
        std::vector<std::function<std::unique_ptr<ISystem>()>> m_systemFactories;
        std::vector<std::type_index> m_systemTypes;
        std::vector<std::function<void(Scene&)>> m_initializeCallbacks;
        std::vector<std::function<void(Scene&)>> m_loadCallbacks;
        std::vector<std::function<void(Scene&)>> m_unloadCallbacks;

        bool m_isStartupScene = false;
        bool m_isPersistent = false;  // Bleibt geladen bei Scene-Wechsel

        /**
         * Erstellt eine neue Scene-Instanz basierend auf dieser Definition
         */
        std::unique_ptr<Scene> instantiate() const;
    };
}