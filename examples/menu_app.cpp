#include "menu_app.hpp"

#include "../src/ender/ender.hpp"
#include "../src/ender/platform/internet.hpp"

// ImGui for creating the menu.
#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>

bool menu_app::on_create_handler(ender::game_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    return app->on_create();
}

void menu_app::on_destroy_handler(ender::game_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    app->on_destroy();
}

bool menu_app::on_handle_events_handler(ender::game_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    return app->on_handle_events();
}

void menu_app::on_render_frame_handler(ender::game_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    app->on_render_frame_imgui();
}

bool menu_app::on_message_close_handler(ender::game_window* ctx) {
    // Return true to confirm user exit with dialogue box in wndproc.
    return true;
}

bool menu_app::menu_app::on_create() noexcept {
    if (ender::use_imgui == true) {
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 32.0f);
        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 16.0f);

        io.Fonts->Build();

        ImGui_ImplDX11_InvalidateDeviceObjects();
        ImGui_ImplDX11_CreateDeviceObjects();

        io.FontDefault = io.Fonts->Fonts[0];
    }

    return true;
}

void menu_app::menu_app::on_destroy() noexcept {
}

bool menu_app::menu_app::on_handle_events() noexcept {
    if (m_is_running == false) {
        return false;
    }

    return true;
}

void menu_app::menu_app::on_render_frame_imgui() noexcept {
    if constexpr (ender::use_imgui == true) {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font_big = io.Fonts->Fonts[0];
        ImFont* font_small = io.Fonts->Fonts[1];

        const ender::vec2i client_size = get_client_size();

        // Set the next window to the size of the client draw area.
        ImGui::SetNextWindowSize(
            ImVec2{static_cast<float>(client_size.x), static_cast<float>(client_size.y)});
        ImGui::SetNextWindowPos({0, 0});

        if (ImGui::Begin("menu app", nullptr,
                         ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                             ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize) == true) {
            switch (m_current_page) {
                case pages::login: {
                    ImGui::PushFont(font_big);
                    ImGui::Text("Welcome");
                    ImGui::Separator();
                    ImGui::PopFont();

                    ImGui::PushFont(font_small);

                    static char username[32] = {};
                    ImGui::InputText("username", username, IM_ARRAYSIZE(username));

                    if (ImGui::Button("login") == true) {
                        m_current_page = pages::home;
                    }

                    static bool remember_me_button = true;
                    ImGui::Checkbox("remember me", &remember_me_button);

                    ImGui::PopFont();
                } break;
                case pages::home: {
                    ImGui::PushFont(font_big);
                    ImGui::Text("Home");
                    ImGui::PopFont();

                    ImGui::PushFont(font_small);
                    if (ImGui::Button("logout") == true) {
                        m_current_page = pages::login;
                    }
                    ImGui::PopFont();
                } break;
            }

            ImGui::End();
        }
    }
}
