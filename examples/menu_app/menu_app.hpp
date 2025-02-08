#pragma once
#include "../../ender/platform/window.hpp"

namespace menu_app {
    class simple_window : public ender::window {
    public:
        simple_window() : window() {
        }

        // Called when the window has been created.
        bool on_create() noexcept;

        // Render imgui here.
        void on_render_frame_imgui() noexcept;
    };

    bool on_create_handler(ender::window* ctx);
    void on_render_frame_handler(ender::window* ctx);

    int run_menu_app();
}  // namespace menu_app
