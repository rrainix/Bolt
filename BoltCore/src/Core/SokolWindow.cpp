#include "Bolt/btpch.hpp"
#define SOKOL_IMPL
#define SOKOL_NO_ENTRY
#define SOKOL_NO_GLLOADER
#define SOKOL_GLCORE

#include "Bolt/Core/SokolWindow.hpp"

#include <bgfx/platform.h>
#include <bimg/decode.h>
#include <bx/allocator.h>

namespace Bolt {

    SokolWindow::SokolWindow() = default;
    SokolWindow::SokolWindow(const sapp_desc& d) : m_desc(d) {}

    void SokolWindow::Run() {
        // Trampoline festlegen
        m_desc.init_cb = &SokolWindow::s_init;
        m_desc.frame_cb = &SokolWindow::s_frame;
        m_desc.cleanup_cb = &SokolWindow::s_cleanup;
        m_desc.event_cb = &SokolWindow::s_event;
        m_desc.user_data = this;
        sapp_run(&m_desc);
    }

    bgfx::PlatformData SokolWindow::PlatformData() const {
        bgfx::PlatformData pd{};
#if defined(_WIN32)
        pd.nwh = const_cast<void*>(sapp_win32_get_hwnd());
#elif defined(__APPLE__)
        pd.nwh = sapp_macos_window();
#elif defined(__linux__)
        pd.ndt = sapp_x11_display();
        pd.nwh = sapp_x11_window();
#endif
        return pd;
    }

    SokolWindow* SokolWindow::Self() {
        return static_cast<SokolWindow*>(sapp_userdata());
    }

    void SokolWindow::s_init() {
        if (auto* w = Self(); w && w->m_init) w->m_init();
    }

    void SokolWindow::s_frame() {
        if (auto* w = Self(); w && w->m_frame) w->m_frame();
    }

    void SokolWindow::s_cleanup() {
        if (auto* w = Self(); w && w->m_cleanup) w->m_cleanup();
    }

    void SokolWindow::s_event(const sapp_event* ev) {
        if (!ev) return;
        if (auto* w = Self()) {
            // General Event
            if (w->m_event)        w->m_event(*ev);
            // Resize
            if (ev->type == SAPP_EVENTTYPE_RESIZED) {
                if (w->m_resize)    w->m_resize(ev->window_width, ev->window_height);
            }
            // Key down
            if (ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
                if (w->m_keyDown)  w->m_keyDown(ev->key_code, ev->modifiers);
            }
            // Key up
            if (ev->type == SAPP_EVENTTYPE_KEY_UP) {
                if (w->m_keyUp)    w->m_keyUp(ev->key_code, ev->modifiers);
            }
            // Char
            if (ev->type == SAPP_EVENTTYPE_CHAR) {
                if (w->m_charInput) w->m_charInput(static_cast<char>(ev->char_code));
            }
            // Mouse down
            if (ev->type == SAPP_EVENTTYPE_MOUSE_DOWN) {
                if (w->m_mouseDown) w->m_mouseDown(ev->mouse_button);
            }
            // Mouse up
            if (ev->type == SAPP_EVENTTYPE_MOUSE_UP) {
                if (w->m_mouseUp)   w->m_mouseUp(ev->mouse_button);
            }
            // Mouse move
            if (ev->type == SAPP_EVENTTYPE_MOUSE_MOVE) {
                if (w->m_mouseMove) w->m_mouseMove(ev->mouse_x, ev->mouse_y);
            }
            // Scroll
            if (ev->type == SAPP_EVENTTYPE_MOUSE_SCROLL) {
                if (w->m_scroll)    w->m_scroll(ev->scroll_x, ev->scroll_y);
            }
        }
    }

    Vec2Int SokolWindow::Size() const { return { sapp_width(), sapp_height() }; }
    int SokolWindow::Width() const { return sapp_width(); }
    int SokolWindow::Height() const { return sapp_height(); }

    bool SokolWindow::IsFullscreen() const { return sapp_is_fullscreen(); }

    void SokolWindow::SetFullscreen(bool enable) {
        if (sapp_is_fullscreen() != enable)
        {
            sapp_toggle_fullscreen();
            m_resize(Width(), Width());
        }
    }
    void SokolWindow::SetWindowTitle(const std::string& title) {
        sapp_set_window_title(title.c_str());
    }
    void SokolWindow::SetClipboardString(const std::string& str) {
        sapp_set_clipboard_string(str.c_str());
    }

    std::vector<uint8_t> SokolWindow::LoadFile(const char* path) {
        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file) return {};

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        if (size <= 0) return {};

        std::vector<uint8_t> data(static_cast<std::size_t>(size));
        file.read(reinterpret_cast<char*>(data.data()), size);
        return data;
    }
    void SokolWindow::SetWindowIcon(const char* filepath) {
        auto fileData = LoadFile(filepath);
        if (fileData.empty()) return;


        static bx::DefaultAllocator allocator;
        bimg::ImageContainer* image = bimg::imageParse(
            &allocator,
            fileData.data(),
            static_cast<uint32_t>(fileData.size())
        );
        if (!image) return;


        sapp_icon_desc iconDesc;
        iconDesc.sokol_default = false;

        for (int i = 0; i < SAPP_MAX_ICONIMAGES; ++i) {
            iconDesc.images[i].width = 0;
            iconDesc.images[i].height = 0;
            iconDesc.images[i].pixels.ptr = nullptr;
            iconDesc.images[i].pixels.size = 0;
        }

        iconDesc.images[0].width = static_cast<int>(image->m_width);
        iconDesc.images[0].height = static_cast<int>(image->m_height);
        iconDesc.images[0].pixels.ptr = image->m_data;
        iconDesc.images[0].pixels.size = static_cast<size_t>(image->m_width) * static_cast<size_t>(image->m_height) * 4;


        sapp_set_icon(&iconDesc);
        bimg::imageFree(image);
    }

    void SokolWindow::SetCursor(const sapp_mouse_cursor& cursor) {
        sapp_set_mouse_cursor(cursor);
    }
}