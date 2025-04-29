#pragma once
#include "../platform/window.hpp"

namespace examples {
    /*
     * @brief Simple window class example.
     *
     * One of the simplest possible implementations of ender::window.
     * This can be used as a boilerplate template for the framework.
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
