#include "Bolt/btpch.hpp"
#include "Bolt/Core/Application.hpp"

#include "Bolt/Scene/SceneManager.hpp"
#include "Bolt/Scene/Scene.hpp"

#include "Bolt/Core/Input.hpp"
#include "Bolt/Core/Time.hpp"

#include "Bolt/Graphics/Camera2D.hpp"

#include "Bolt/Components/SpriteRenderer.hpp"
#include "Bolt/Collections/Color.hpp"

#include "Bolt/Debug/Logger.hpp"

#include "Bolt/Audio/AudioManager.hpp"
#include "Bolt/Audio/AudioSource.hpp"

#include "Bolt/Collections/Viewport.hpp"

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <bgfx/platform.h>
#include <chrono>
#define NULLPTR = nullptr;

namespace Bolt {

	Application::Application()
	{
		AudioManager::Initialize();
	}

	float Application::s_TargetFramerate = 144.0f;

	void Application::Run() {
		sapp_desc d{};
		d.width = 800;
		d.height = 800;
		d.window_title = "Bolt Engine";
		d.enable_clipboard = true;

		m_Window = SokolWindow(d);

		m_Window.SetInit([&] {
			Initialize();
			});

		using clock = std::chrono::high_resolution_clock;
		using dur = clock::duration;

		float fixedUpdateAccumulator = 0.0f;

		m_Window.SetFrame([&] {
			dur TARGET_FRAME_TIME = std::chrono::duration_cast<dur>(
				std::chrono::duration<double>(1.0 / s_TargetFramerate)
			);
			static auto lastTime = clock::now();
			auto const nextFrameTime = lastTime + TARGET_FRAME_TIME;

			auto now = clock::now();
			if (now + std::chrono::milliseconds(10) < nextFrameTime) {
				std::this_thread::sleep_until(nextFrameTime - std::chrono::milliseconds(10));
			}
			while (clock::now() < nextFrameTime) {
				_mm_pause();
			}
			auto frameStart = clock::now();

			float deltaTime = std::chrono::duration<float>(frameStart - lastTime).count();
			Time::Update(deltaTime);

			fixedUpdateAccumulator += Time::GetDeltaTime();
			while (fixedUpdateAccumulator >= Time::s_FixedDeltaTime) {
				{
					try {
						FixedUpdate();
					}
					catch (std::runtime_error e) {
						Logger::Error(e.what());
					}
				}

				fixedUpdateAccumulator -= Time::s_FixedDeltaTime;
			}

			Update();
			lastTime = frameStart;
			});
		m_Window.SetCleanup([&] {
			Quit();
			});
		m_Window.SetResize([this](int w, int h) {
			m_Renderer2D.OnResize(w, h);
			});

		m_Window.SetKeyDown([this](sapp_keycode key, int mod) {
			Input::OnKeyDown(key);
			});
		m_Window.SetKeyUp([this](sapp_keycode key, int mod) {
			Input::OnKeyUp(key);
			});

		m_Window.SetMouseDown([&](int btn) {
			Input::OnMouseDown(btn);
			});
		m_Window.SetMouseUp([&](int btn) {
			Input::OnMouseUp(btn);
			});
		m_Window.SetScroll([&](float dx, float dy) {
			Input::OnScroll(dy);
			});
		m_Window.SetMouseMove([&](float x, float y) {
			Input::OnMouseMove(x, y);
			});

		m_Window.SetCharInput([this](char c) {

			});

		m_Window.Run();
	}

	void Application::Initialize() {
		const auto pd = m_Window.PlatformData();
		const auto size = m_Window.Size();

		m_Viewport = std::make_shared<Bolt::Viewport>(size.x, size.y);
		Camera2D::m_Viewport = m_Viewport;

		if (!m_Renderer2D.Initialize(pd, size.x, size.y)) {
			return;
		}
		m_Window.SetResize([this](int w, int h) {
			m_Viewport->SetSize(w, h);
			m_Renderer2D.OnResize(w, h);
			});


		m_Window.SetWindowIcon("Assets/Textures/bolt_logo_3.png");
	}

	void Application::WindowControlls() {
		if (Input::GetKeyDown(KeyCode::Esc)) {
			sapp_request_quit();
			return;
		}
		if (Input::GetKeyDown(KeyCode::F11)) {
			m_Window.SetFullscreen(!m_Window.IsFullscreen());
			m_Renderer2D.ShouldRebuildStaticBatches();
		}

		if (Input::GetMouse(0)) m_Window.SetCursor(sapp_mouse_cursor::SAPP_MOUSECURSOR_POINTING_HAND);
		else m_Window.SetCursor(sapp_mouse_cursor::SAPP_MOUSECURSOR_DEFAULT);

		m_Window.SetWindowTitle(std::to_string(1.0f / Time::GetDeltaTimeUnscaled()) + " FPS" + std::to_string(1.0f / sapp_frame_duration()) + " SFPS TARGET FPS:" + std::to_string(s_TargetFramerate));
	}

	void Application::FixedUpdate() {
		SceneManager::FixedUpdateScenes();
		m_PhysicsSystem.FixedUpdate(Time::GetFixedDeltaTime());
	}

	void Application::Update() {
		AudioManager::Update();
		WindowControlls();

		if (Camera2D::Main() == nullptr) {
			Logger::Warning("Camera is null");
		}

		m_Renderer2D.BeginFrame();
		SceneManager::UpdateScenes();
		m_Renderer2D.EndFrame();
		Input::Update();
	}

	void Application::Quit() {
		Logger::Message("Quit");
		AudioManager::Shutdown();
		m_Renderer2D.Shutdown();
	}
}