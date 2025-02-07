#include "../src/platform/window.hpp"

#include <imgui\imgui.h>

// Inheret from ender::platform_window to easily spawn a window.
class menu_app : public ender::platform_window {
public:
    menu_app() : platform_window() {
    }

    // Called when the window has been created.
    bool on_create() noexcept {
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

    // Render imgui here.
    void on_render_frame_imgui() noexcept {
        ImGui::ShowDemoWindow();
    }
};

bool on_create_handler(ender::platform_window* ctx) {
    auto app = static_cast<menu_app*>(ctx);
    return app->on_create();
}

// These are the callbacks given to the window.
// Use them to call custom events in the class.
void on_render_frame_handler(ender::platform_window* ctx) {
    auto app = static_cast<menu_app*>(ctx);

    // Render ImGui event.
    app->on_render_frame_imgui();

    // Call some other custom event in your class.
    // app->on_render_frame()
}

// Example Windows entry point.
INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    // Create and run the window.
    // Put more windows in seperate threads...
    auto app = std::make_unique<menu_app>();
    if (app->create(on_create_handler, {.title = L"menu app",
                                        .width = 1280,
                                        .height = 720,
                                        .on_message_create = nullptr,
                                        .on_message_destroy = nullptr,
                                        .on_message_close = nullptr,
                                        .instance = instance,
                                        .cmd_show = cmd_show}) == true) {
        while (app->handle_events(nullptr) == true) {
            app->render_frame(on_render_frame_handler);
        }

        app->destroy(nullptr);
    }

    return 0;
}
