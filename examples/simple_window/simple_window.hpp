#pragma once
#include "../platform/window.hpp"

namespace examples {
    /*
     * @brief Simple window example.
     *
     * This is an extremely simple window that uses the ender::window class.
     * It shows how to create a window and render a frame with ImGui.
     *
     * @note This can be used as a boilerplate example for the ender framework.
     */
    class simple_window final : public ender::window {
    public:
        simple_window() : window() {
        }

        bool create(window_details details) noexcept;
        bool destroy() noexcept;
        bool process_events() noexcept;
        void render_frame() noexcept;
    };

    int run_simple_window();
}  // namespace examples
