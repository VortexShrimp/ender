#include "menu_app.hpp"

#include "../src/ender/ender.hpp"
#include "../src/ender/platform/internet.hpp"

// ImGui for creating the menu.
#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>

bool menu_app::on_create_handler(ender::platform_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    return app->on_create();
}

void menu_app::on_destroy_handler(ender::platform_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    app->on_destroy();
}

bool menu_app::on_handle_events_handler(ender::platform_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    return app->on_handle_events();
}

void menu_app::on_render_frame_handler(ender::platform_window* ctx) {
    auto* app = static_cast<menu_app*>(ctx);
    app->on_render_frame_imgui();
}

bool menu_app::on_message_close_handler(ender::platform_window* ctx) {
    // Return true to confirm user exit with dialogue box in wndproc.
    return true;
}

bool menu_app::menu_app::on_create() noexcept {
    if (m_console.create() == false) {
        return false;
    }

    m_console.set_title("menu_app logger");

    if (ender::use_imgui == true) {
        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 32.0f);
        io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 16.0f);

        io.Fonts->Build();

        ImGui_ImplDX11_InvalidateDeviceObjects();
        ImGui_ImplDX11_CreateDeviceObjects();

        io.FontDefault = io.Fonts->Fonts[0];
    }

    m_lua_state.open_libraries(sol::lib::base);
    m_lua_state.script_file("scripts\\example.ender");

    m_console.write("[menu_app::on_create] Success.\n");

    return true;
}

void menu_app::menu_app::on_destroy() noexcept {
    m_console.destroy();
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
            if (ImGui::BeginChild("##child") == true) {
                switch (m_current_page) {
                    case pages::login: {
                        ImGui::PushFont(font_big);
                        ImGui::Text("Welcome");
                        ImGui::Separator();
                        ImGui::PopFont();

                        ImGui::PushFont(font_small);

                        ImGui::InputText("username", m_username_buffer.data(),
                                         m_username_buffer.size());

                        if (ImGui::Button("login") == true) {
                            m_current_page = pages::home;

                            std::string username = m_username_buffer.data();
                            m_console.write("Logged in as '{}'\n", username);
                        }

                        ImGui::Checkbox("remember me", &m_should_remember_me);

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
                ImGui::EndChild();
            }
            ImGui::End();
        }
    }
}
