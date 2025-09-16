#pragma once
#include "Bolt/collections/Vec2.hpp"

#include <sokol/sokol_app.h>

#include <functional>
#include <string>
#include <fstream>
#include <vector>


namespace bgfx { struct PlatformData; }

namespace Bolt {

	class SokolWindow {
	public:
		// Callback-Typen
		using InitFn = std::function<void()>;
		using FrameFn = std::function<void()>;
		using CleanupFn = std::function<void()>;
		using EventFn = std::function<void(const sapp_event&)>;
		using ResizeFn = std::function<void(int, int)>;
		using KeyFn = std::function<void(sapp_keycode, int)>;
		using CharFn = std::function<void(char)>;
		using MouseBtnFn = std::function<void(int /*button*/)>;
		using MouseMoveFn = std::function<void(double /*x*/, double /*y*/)>;
		using ScrollFn = std::function<void(double /*dx*/, double /*dy*/)>;

		SokolWindow();
		explicit SokolWindow(const sapp_desc& desc);

		void SetInit(InitFn cb) { m_init = std::move(cb); }
		void SetFrame(FrameFn cb) { m_frame = std::move(cb); }
		void SetCleanup(CleanupFn cb) { m_cleanup = std::move(cb); }
		void SetEvent(EventFn cb) { m_event = std::move(cb); }
		void SetResize(ResizeFn cb) { m_resize = std::move(cb); }
		void SetKeyDown(KeyFn cb) { m_keyDown = std::move(cb); }
		void SetKeyUp(KeyFn cb) { m_keyUp = std::move(cb); }
		void SetCharInput(CharFn cb) { m_charInput = std::move(cb); }
		void SetMouseDown(MouseBtnFn cb) { m_mouseDown = std::move(cb); }
		void SetMouseUp(MouseBtnFn cb) { m_mouseUp = std::move(cb); }
		void SetMouseMove(MouseMoveFn cb) { m_mouseMove = std::move(cb); }
		void SetScroll(ScrollFn cb) { m_scroll = std::move(cb); }

		void Run();


		bgfx::PlatformData PlatformData() const;
		Vec2Int Size() const;
		int Width() const;
		int Height() const;

		bool IsFullscreen() const;

		void SetFullscreen(bool enable = true);
		void SetWindowTitle(const std::string& title);
		void SetClipboardString(const std::string& str);

		std::vector<uint8_t> LoadFile(const char* path);
		void SetWindowIcon(const char* filepath);

		void SetCursor(const sapp_mouse_cursor& cursor);


		sapp_desc& Desc() { return m_desc; }
		const sapp_desc& Desc() const { return m_desc; }

	private:
		static void s_init();
		static void s_frame();
		static void s_cleanup();
		static void s_event(const sapp_event* ev);
		static SokolWindow* Self();

		sapp_desc    m_desc{};
		InitFn       m_init;
		FrameFn      m_frame;
		CleanupFn    m_cleanup;
		EventFn      m_event;
		ResizeFn     m_resize;
		KeyFn        m_keyDown;
		KeyFn        m_keyUp;
		CharFn       m_charInput;
		MouseBtnFn   m_mouseDown;
		MouseBtnFn   m_mouseUp;
		MouseMoveFn  m_mouseMove;
		ScrollFn     m_scroll;
	};

}