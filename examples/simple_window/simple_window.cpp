#include "simple_window.hpp"

#include "../ender.hpp"

#include <imgui\imgui.h>

bool examples::simple_window::create(window_details details) noexcept {
    const bool result = window::on_create(details);

    ImFont* font =
        ImGui::GetIO().Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 22.0f);
    ImGui::GetIO().FontDefault = font;

    // Your custom init code here
    // ...

    return result;
}

bool examples::simple_window::destroy() noexcept {
    // Free your custom window code here
    // ...

    return window::on_destroy();
}

bool examples::simple_window::process_events() noexcept {
    const bool should_run = window::on_process_events();

    // Custom processing here
    // ...

    return should_run;
}

void examples::simple_window::render_frame() noexcept {
    window::on_pre_render_frame();

    // Your custom render code here
    // ...

#ifdef ENDER_IMGUI
    ImGui::ShowDemoWindow(&m_is_running);

    ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255),
                                            "Hello from ImGui!");
#endif  // ENDER_IMGUI

    window::on_post_render_frame();
}

int examples::run_simple_window() {
    // Create and run the window.
    auto app = std::make_unique<simple_window>();
    if (app->create({.title = L"simple window (example)",
                     .class_name = L"simple_window",
                     .style = ender::window_style::borderless,
                     .width = 1280,
                     .height = 720,
                     .on_message_create = nullptr,
                     .on_message_destroy = nullptr,
                     .on_message_close = nullptr}) == true) {
        while (app->process_events() == true) {
            app->render_frame();
        }

        if (app->destroy() == true) {
            return 0;
        }
    }

    return 1;
}
