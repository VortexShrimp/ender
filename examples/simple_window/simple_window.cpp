#include "simple_window.hpp"

#include "../utils/console.hpp"

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
    ender::debug_print_raw("[simple_window] Shutting down.\n");

    return window::on_destroy();
}

bool examples::simple_window::process_events() noexcept {
    return window::on_process_events();
}

void examples::simple_window::render_frame() noexcept {
    window::on_pre_render_frame();

#ifdef ENDER_IMGUI
    ImGui::ShowDemoWindow(&m_is_running);

    ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 10), IM_COL32(255, 255, 255, 255),
                                            "Hello from ImGui!");

    // Use these settings with borderless windows.
    // auto [x, y] = get_client_size();
    // ImGui::SetNextWindowSize({static_cast<float>(x), static_cast<float>(y)});
    // ImGui::SetNextWindowPos({0, 0});
    // ImGui::Begin("Hello from ImGui!", &m_is_running,
    //              ImGuiWindowFlags_NoResize | ImGuiWindowFlags_None |
    //              ImGuiWindowFlags_NoCollapse);
    // ImGui::End();
#endif  // ENDER_IMGUI

    window::on_post_render_frame();
}

int examples::run_simple_window() {
    // Create and run the window.
    // You can put more windows in seperate threads...
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
