#pragma once

#include "Bolt/Core/Input.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>

namespace Bolt {

	enum WindowMode { FULLSCREEN, WINDOWED_MAXIMIZED, WINDOWED_FREE };

	class Window {
	public:
		Window(int width, int height, const std::string& title, WindowMode windowMode);
		~Window();

		// Verhindern von Kopieren der Instanz
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;


		GLFWwindow* glfwWindow() const { return m_window; }
		float aspect() const { return static_cast<float>(m_width) / static_cast<float>(m_height); }
		glm::ivec2 size() const { return { m_width, m_height }; }

		static void vsync(bool enabled) { glfwSwapInterval(enabled ? 1 : 0); };
		void setWindowResizeable(bool enabled) { glfwWindowHint(GLFW_RESIZABLE, enabled ? GLFW_TRUE : GLFW_FALSE); }
		void setWindowMoveable(bool enabled) { glfwWindowHint(GLFW_DECORATED, enabled ? GLFW_TRUE : GLFW_FALSE); }

		void maximizeWindow();
		void minimizeWindow();
		bool isMaximized() const;
		bool isMinimized() const;
		void centerWindow();
		void focusWindow();
		static Window& activeWindow() { return *s_activeWindow; }

	private:
		void initWindow();
		void updateViewport();
		void updateWindowSize();
		bool shouldClose() const { return glfwWindowShouldClose(m_window); }
		void swapBuffers() const { glfwSwapBuffers(m_window); }
		bool isWindowResized() const { return m_windowResized; }
		void resetWindowResizedFlag() { m_windowResized = false; }

		glm::ivec2 center() const;


		// Callbacks
		static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
		static void windowResizedCallback(GLFWwindow* window, int width, int height);

		// Fensterdetails
		static Window* s_activeWindow;

		GLFWwindow* m_window = nullptr;
		GLFWmonitor* m_monitor = nullptr;
		const GLFWvidmode* k_mode = nullptr;

		// Fenstergrößen
		int m_width, m_height;
		std::string m_title;
		bool m_windowResized = false;
		WindowMode m_windowMode;

		friend class Application;
	};
}