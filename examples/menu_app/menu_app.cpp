#include "menu_app.hpp"

#include "../src/utils/console.hpp"

#include <imgui\imgui.h>

bool menu_app::simple_window::on_create() noexcept {
    ender::debug_print_formatted("[menu_app] on_create");

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
    if constexpr (ender::use_imgui == true) {
        ImGui::ShowDemoWindow();
    }
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
    ender::debug_print_raw("Welcome to Menu App!\n");

    // Create and run the window.
    // Put more windows in seperate threads...
    auto app = std::make_unique<simple_window>();
    if (app->create(on_create_handler, {.title = L"menu app",
                                        .class_name = L"menu_app",
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
