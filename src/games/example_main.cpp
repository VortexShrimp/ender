#include <thread>

#include "../engine/ender.hpp"
#include "../engine/platform/console.hpp"
#include "../engine/platform/window.hpp"

#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>

namespace game {
    bool window_create(ender::game_window* window) {
        // Initialize game stuff here...
        window->set_title(L"ender example");

        if (ender::use_imgui == true) {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 32.0f);
            io.Fonts->Build();

            ImGui_ImplDX11_InvalidateDeviceObjects();
            ImGui_ImplDX11_CreateDeviceObjects();

            io.FontDefault = io.Fonts->Fonts[0];
        }

        return true;
    }

    bool handle_events(ender::game_window* window) {
        // Return false to exit the game.
        if (window->is_running() == false) {
            return false;
        }

        return true;
    }

    void render_start_menu(ender::game_window* window) {
        if constexpr (ender::use_imgui == true) {
            int width = 0;
            int height = 0;
            window->get_client_size(width, height);

            ImGui::SetNextWindowSize({static_cast<float>(width), static_cast<float>(height)});
            ImGui::SetNextWindowPos({0, 0});
            if (ImGui::Begin("ender menu", nullptr,
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize) == true) {
                ImGui::Text("ender example");
                ImGui::Button("begin", {250, 100});
                ImGui::Text("%f", window->get_delta_time());
                if (ImGui::Button("exit", {250, 100}) == true) {
                    window->stop_running();
                }
                ImGui::End();
            }
        }
    }

    void render_game(ender::game_window* window) {
        if constexpr (ender::use_imgui == true) {
            ImGui::GetBackgroundDrawList()->AddRectFilled({20, 20}, {300, 300},
                                                          ImColor(255, 0, 0, 255));
        }
    }

    void window_destroy(ender::game_window* window) {
    }
}  // namespace game

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    ender::game_window main_window = {};
    if (main_window.create(game::window_create, {.title = L"ender",
                                                 .width = 1280,
                                                 .height = 720,
                                                 .on_message_create = nullptr,
                                                 .on_message_destroy = nullptr,
                                                 .instance = instance,
                                                 .cmd_show = cmd_show}) == true) {
        while (main_window.handle_events(game::handle_events) == true) {
            main_window.render_frame(game::render_start_menu);
        }

        main_window.destroy(game::window_destroy);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
