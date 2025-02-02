#pragma once
#include <string_view>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIn32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>

#include "../utils/pointers.hpp"

namespace ender {
    class engine_renderer;
    class engine_window {
    public:
        using message_create_function = void (*)();
        using message_destroy_function = void (*)();

        using create_function = bool (*)(engine_window* window);  // Return true for success.
        using destroy_function = void (*)(engine_window* window);
        using handle_events_function = bool (*)(engine_window* window);
        using render_frame_function = void (*)(engine_window* window);

        struct window_details {
            std::wstring_view title;
            int width;
            int height;
            message_create_function on_message_create;
            message_destroy_function on_message_destroy;
            HINSTANCE instance;
            int cmd_show;
        };

        engine_window()
            : m_is_running(false),
              m_hwnd(nullptr),
              m_wcex(),
              m_instance(nullptr),
              m_renderer(nullptr) {
        }

        /**
         * @brief Creates the window and renderer.
         * @param on_create Callback called after successful window creation.
         * @param details Parameters for the window.
         * @return True on success.
         */
        bool create(create_function on_create, window_details details);
        bool destroy(destroy_function on_destroy);

        /**
         * @brief
         * @param on_process_input Called after system handles messages.
         * @return True to keep the game running.
         */
        bool handle_events(handle_events_function on_handle_events);
        void render_frame(render_frame_function on_render_frame);

        bool set_title(std::wstring_view new_title);

        bool is_running() const noexcept;
        void stop_running() noexcept;

        void get_client_size(int& width, int& height);
        void get_window_size(int& width, int& height);

    private:
        bool m_is_running;

        HWND m_hwnd;
        ATOM m_wcex;
        HINSTANCE m_instance;

        unique_pointer<engine_renderer> m_renderer;
    };
}  // namespace ender
