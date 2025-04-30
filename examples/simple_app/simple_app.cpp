#include "simple_app.hpp"

#include "../ender.hpp"
#include "../utils/console.hpp"

#include <imgui\imgui.h>

bool examples::simple_app::create(window_details details) noexcept {
    const bool result = window::on_create(details);

#ifdef ENDER_IMGUI
    // Load a custom font.
    ImFont* font =
        ImGui::GetIO().Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 28.0f);
    ImGui::GetIO().FontDefault = font;
#endif ENDER_IMGUI

    return result;
}

bool examples::simple_app::destroy() noexcept {
    return window::on_destroy();
}

bool examples::simple_app::process_events() noexcept {
    return window::on_process_events();
}

void examples::simple_app::render_frame() noexcept {
    window::on_pre_render_frame();

#ifdef ENDER_IMGUI
    // Set the ImGui window size to the size of the client area.
    const auto [width, height] = get_client_size();
    ImGui::SetNextWindowSize({static_cast<float>(width), static_cast<float>(height)});
    ImGui::SetNextWindowPos({0, 0});
    if (ImGui::Begin("Borderless Example", &m_is_running,
                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {
        ImGui::Text("This is a simple window example.");
        if (ImGui::Button("Here's a button") == true) {
            // Do button logic..
        }
        const float delta_time = get_delta_time_seconds();

        ImGui::Text("FPS -> %f", static_cast<float>(1 / delta_time));
        ImGui::Text("DT -> %f", delta_time);
        ImGui::End();
    }
#endif  // ENDER_IMGUI

    window::on_post_render_frame();
}

int examples::run_simple_app() {
    auto app = std::make_unique<simple_app>();
    if (app->create({.title = L"simple app (example)",
                     .class_name = L"simple_app",
                     .style = ender::window_style::resizable,
                     .width = 300,
                     .height = 450,
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
