#pragma once
#include <string_view>
#include <memory>

#ifndef WIN32_LEAN_AND_MEAN
#define WIn32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <directxmath.h>

#include <imgui\imgui.h>

#include "renderer.hpp"
#include "../utils/timer.hpp"

namespace ender {
    /**
     * @brief Spawn a platform window.
     *
     * All windows of this class use the same wndproc that dispatches messages
     * to the individual windows. This data is stored in s_window_data.
     *
     */
    class window {
    public:
        using message_create_function = void (*)();
        using message_destroy_function = void (*)();
        using message_close_function = bool (*)(window* game);  // Return true to confirm exit.

        using create_function = bool (*)(window* game);
        using destroy_function = void (*)(window* game);
        using process_events_function = bool (*)(window* game);
        using render_frame_function = void (*)(window* game);

        struct window_details {
            std::wstring_view title;
            std::wstring_view class_name;

            int width;
            int height;

            message_create_function on_message_create;
            message_destroy_function on_message_destroy;
            message_close_function on_message_close;
        };

        window()
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
        bool process_events(process_events_function on_process_events);

        /**
         * @brief
         * @param on_render_frame
         */
        void render_frame(render_frame_function on_render_frame);

        /**
         * @brief Is the window supposed to be running?
         * @return True if the window should run.
         */
        bool is_running() const noexcept;

    protected:
        bool set_title(std::wstring_view new_title);
        std::wstring_view get_title() const;

        /**
         * @brief Size of client draw area.
         * @return
         */
        DirectX::XMINT2 get_client_size() const noexcept;

        /**
         * @brief Size of entire window.
         * @return
         */
        DirectX::XMINT2 get_window_size() const noexcept;

        float delta_time();

        bool m_is_running;
        std::unique_ptr<d3d11_renderer> m_renderer;

    private:
        HWND m_hwnd;           // Win32 handle to the window.
        ATOM m_wcex;           // Window class ATOM.
        HINSTANCE m_instance;  // Instance (if any) that the window belongs to.

        high_resolution_timer m_timer;
    };

    /**
     * @brief Data that needs to be shared between Window and Message Loop.
     */
    struct internal_window_data {
        window* window;  // Reference to the ender window incase it's needed.

        UINT resize_width;
        UINT resize_height;

        window::message_create_function on_message_create;
        window::message_destroy_function on_message_destroy;
        window::message_close_function on_message_close;
    };

    /**
     * @brief A reference to a window's internal data shared between wndproc and window.
     * @param hwnd Handle to window for data.
     * @return
     */
    internal_window_data& get_internal_window_data(HWND hwnd);
}  // namespace ender
