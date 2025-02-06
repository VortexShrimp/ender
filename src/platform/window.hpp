#pragma once
#include <string_view>

#ifndef WIN32_LEAN_AND_MEAN
#define WIn32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <sol\sol.hpp>

#include "renderer.hpp"
#include "../utils/pointers.hpp"
#include "../utils/timer.hpp"
#include "../math/vectors.hpp"

namespace ender {
    class platform_window {
    public:
        using message_create_function = void (*)();
        using message_destroy_function = void (*)();
        using message_close_function =
            bool (*)(platform_window* game);  // Return true to confirm exit.

        using create_function = bool (*)(platform_window* game);
        using destroy_function = void (*)(platform_window* game);
        using handle_events_function = bool (*)(platform_window* game);
        using render_frame_function = void (*)(platform_window* game);

        struct window_details {
            std::wstring_view title;
            int width;
            int height;

            message_create_function on_message_create;
            message_destroy_function on_message_destroy;
            message_close_function on_message_close;

            HINSTANCE instance;
            int cmd_show;
        };

        platform_window()
            : m_is_running(false),
              m_hwnd(nullptr),
              m_wcex(),
              m_instance(nullptr),
              m_renderer(nullptr),
              m_timer() {
        }

        /**
         * @brief Creates the window and renderer.
         * @param on_create Callback called after successful window creation.
         * @param details Parameters for the window.
         * @return True on success.
         */
        bool create(create_function on_create, window_details details);

        /**
         * @brief
         * @param on_destroy Called before internal resources are destroyed.
         * @return
         */
        bool destroy(destroy_function on_destroy);

        /**
         * @brief
         * @param on_process_input Called after system handles messages.
         * @return True to keep the game running.
         */
        bool handle_events(handle_events_function on_handle_events);

        /**
         * @brief
         * @param on_render_frame
         */
        void render_frame(render_frame_function on_render_frame);

        bool is_running() const noexcept;

    protected:
        bool create_lua_state();

        bool set_title(std::wstring_view new_title);
        std::wstring_view get_title() const;

        vec2i get_client_size() const noexcept;
        vec2i get_window_size() const noexcept;

        float get_delta_time();

        bool m_is_running;
        unique_pointer<d3d11_renderer> m_renderer;
        sol::state m_lua_state;

    private:
        HWND m_hwnd;
        ATOM m_wcex;
        HINSTANCE m_instance;

        high_resolution_timer m_timer;
    };

    /**
     * @brief Data that needs to be shared between Window and Message Loop.
     */
    struct internal_window_data {
        platform_window* window;  // Reference to the window.

        UINT resize_width;  // Resizing is handled in render loop and wndproc.
        UINT resize_height;

        platform_window::message_create_function on_message_create;
        platform_window::message_destroy_function on_message_destroy;
        platform_window::message_close_function on_message_close;
    };

    /**
     * @brief A reference to a window's internal data shared between wndproc and window.
     * @param hwnd Handle to window for data.
     * @return
     */
    internal_window_data& get_internal_window_data(HWND hwnd);
}  // namespace ender
