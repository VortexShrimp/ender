#pragma once
#include <string_view>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIn32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d11.h>

#include "../utils/pointers.hpp"
#include "../utils/timer.hpp"
#include "../math/vectors.hpp"

namespace ender {
    class game_renderer;
    class game_window {
    public:
        using message_create_function = void (*)();
        using message_destroy_function = void (*)();

        using create_function = bool (*)(void* game,
                                         game_window* window);  // Return true for success.
        using destroy_function = void (*)(void* game, game_window* window);
        using handle_events_function = bool (*)(void* game, game_window* window);
        using render_frame_function = void (*)(void* game, game_window* window);

        struct window_details {
            std::wstring_view title;
            int width;
            int height;
            message_create_function on_message_create;
            message_destroy_function on_message_destroy;
            HINSTANCE instance;
            int cmd_show;
        };

        game_window()
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
        bool create(void* game, create_function on_create, window_details details);
        bool destroy(void* game, destroy_function on_destroy);

        /**
         * @brief
         * @param on_process_input Called after system handles messages.
         * @return True to keep the game running.
         */
        bool handle_events(void* game, handle_events_function on_handle_events);
        void render_frame(void* game, render_frame_function on_render_frame);

        bool set_title(std::wstring_view new_title);
        std::wstring_view get_title() const;

        bool is_running() const noexcept;
        void stop_running() noexcept;

        vec2i get_client_size() const noexcept;
        vec2i get_window_size() const noexcept;

        float get_delta_time();

    private:
        bool m_is_running;

        HWND m_hwnd;
        ATOM m_wcex;
        HINSTANCE m_instance;

        unique_pointer<game_renderer> m_renderer;
        high_resolution_timer m_timer;
    };

}  // namespace ender
