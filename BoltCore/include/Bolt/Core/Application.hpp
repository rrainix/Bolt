#pragma once
#include "Bolt/Core/SokolWindow.hpp"
#include "Bolt/Graphics/Renderer2D.hpp"
#include "Bolt/Physics/PhysicsSystem.hpp"

namespace Bolt {
	class Shader;
	class Viewport;

	class Application {
	public:
		Application();
		~Application() = default;
		void Run();

		void SetTargetFramerate(float fps) { s_TargetFramerate = fps; }
		float GetTargetFramerate() const { return s_TargetFramerate; }

	private:
		void Initialize();
		void FixedUpdate();
		void Update();
		void Quit();
		void WindowControlls();

		std::shared_ptr<Viewport> m_Viewport;

		SokolWindow      m_Window;
		Renderer2D m_Renderer2D;
		PhysicsSystem m_PhysicsSystem;

		static float s_TargetFramerate;
	};
}