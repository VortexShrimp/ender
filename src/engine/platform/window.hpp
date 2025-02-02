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

    /**
     * @brief Data that needs to be shared between Window and Message Loop.
     */
    struct engine_window_data {
        UINT resize_width;  // Resizing is handled in render loop and wndproc.
        UINT resize_height;

        using message_function_create = void (*)();
        using message_function_destroy = void (*)();

        message_function_create on_create;
        message_function_destroy on_destroy;
    };

    class engine_window {
    public:
        struct window_details {
            std::wstring_view title;
            int width;
            int height;
            HINSTANCE instance;
            int cmd_show;
        };

        using create_function = void (*)();
        using destroy_function = void (*)();
        using process_input_function = void (*)();
        using render_frame_function = void (*)();

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

        void process_input(process_input_function on_process_input);
        void render_frame(render_frame_function on_render_frame);

        bool set_title(std::wstring_view new_title);

        bool is_running() const noexcept;

    private:
        bool m_is_running;

        HWND m_hwnd;
        ATOM m_wcex;
        HINSTANCE m_instance;

        unique_pointer<engine_renderer> m_renderer;
    };
}  // namespace ender
