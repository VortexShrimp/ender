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

/**
 * @brief Used to denote that a function should be overridden.
 *
 * The `ender::window` should be inheretted and must have its own
 * copies of the functions that are marked with this macro.
 *
 * In those copies, the original function must be called. For
 * example `my_window::on_create()` should call `ender::window::on_create()`
 * before running its own logic.
 *
 * @note This macro does nothing. It's just a marker for API users.
 */
#define ENDER_OVERRIDE

namespace ender {
    enum class window_style {
        /**
         * @brief A normal window with a title bar and close button.
         */
        resizable,

        /**
         * @brief A borderless window with no title bar or close button.
         * Currently non-resizable.
         */
        borderless
    };

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
        using message_close_function = bool (*)(window* app);  // Return true to confirm exit.

        struct window_details {
            std::wstring_view title;
            std::wstring_view class_name;

            window_style style;

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

    protected:
        /**
         * @brief Creates the window and renderer.
         * @param details Parameters for the window.
         * @return True on success.
         */
        ENDER_OVERRIDE bool create(window_details details);

        /**
         * @brief Destroys the window and frees its resources.
         * @return True on success.
         */
        ENDER_OVERRIDE bool destroy();

        /**
         * @brief Processes the window messages.
         * @return True if the window should continue running.
         */
        ENDER_OVERRIDE bool process_events();

        ENDER_OVERRIDE void pre_render_frame();

        ENDER_OVERRIDE void post_render_frame();

        /**
         * @brief Is the window supposed to be running?
         * @return
         */
        bool is_running() const noexcept;

        /**
         * @brief Set the window title.
         * @param new_title New title for the window.
         * @return True on success.
         */
        bool set_title(std::wstring_view new_title);

        /**
         * @brief Get the window title.
         * @return Current title of the window.
         */
        std::wstring_view get_title() const;

        /**
         * @brief Size of client draw area.
         * @return
         */
        DirectX::XMINT2 get_client_size() const noexcept;

        /**
         * @brief Size of entire window on the OS.
         * @return
         */
        DirectX::XMINT2 get_window_size() const noexcept;

        float get_delta_time_seconds();

        bool m_is_running;
        std::unique_ptr<d3d11_renderer> m_renderer;

    private:
        HWND m_hwnd;           // Win32 handle to the window.
        ATOM m_wcex;           // Window class ATOM.
        HINSTANCE m_instance;  // Instance (if any) that the window belongs to.

        timer_complex m_timer;  // Timer for the window.
    };

    /**
     * @brief Data that needs to be shared between Window and Message Loop.
     */
    struct internal_window_data {
        window* window;  // Reference to the ender window incase it's needed.

        UINT resize_width;
        UINT resize_height;

        window_style style;
        bool is_dragging;
        POINT drag_start;

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
