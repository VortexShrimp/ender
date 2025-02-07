#include "menu_app.hpp"

#include <imgui\imgui.h>

bool menu_app::simple_window::on_create() noexcept {
    set_title(L"menu app example");

    //// Custom fonts.
    // ImGuiIO& io = ImGui::GetIO();

    // io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 32.0f);
    // io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 16.0f);

    // io.Fonts->Build();

    // ImGui_ImplDX11_InvalidateDeviceObjects();
    // ImGui_ImplDX11_CreateDeviceObjects();

    // io.FontDefault = io.Fonts->Fonts[0];

    return true;
}

// Render ImGui here.
void menu_app::simple_window::on_render_frame_imgui() noexcept {
    ImGui::ShowDemoWindow();
}

bool menu_app::on_create_handler(ender::window* ctx) {
    auto app = static_cast<simple_window*>(ctx);
    return app->on_create();
}

void menu_app::on_render_frame_handler(ender::window* ctx) {
    auto app = static_cast<simple_window*>(ctx);

    // Render ImGui event.
    app->on_render_frame_imgui();

    // Call some other custom event in your class.
    // app->on_render_frame()
}

int menu_app::run_menu_app() {
    // Create and run the window.
    // Put more windows in seperate threads...
    auto app = std::make_unique<simple_window>();
    if (app->create(on_create_handler, {.title = L"menu app",
                                        .width = 1280,
                                        .height = 720,
                                        .on_message_create = nullptr,
                                        .on_message_destroy = nullptr,
                                        .on_message_close = nullptr,
                                        .instance = nullptr,
                                        .cmd_show = SW_SHOWDEFAULT}) == true) {
        while (app->process_events(nullptr) == true) {
            app->render_frame(on_render_frame_handler);
        }

        app->destroy(nullptr);
        return 0;
    }

    return 1;
}
