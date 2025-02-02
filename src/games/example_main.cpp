#include <thread>

#include "../engine/ender.hpp"
#include "../engine/platform/console.hpp"
#include "../engine/platform/window.hpp"

#include <imgui\imgui.h>

namespace game {
    bool on_main_window_create(ender::engine_window* window) {
        // Initialize game stuff here...
        window->set_title(L"ender example");

        return true;
    }

    bool on_main_window_handle_events(ender::engine_window* window) {
        // Return false to exit the game.
        return true;
    }

    void on_main_window_render(ender::engine_window* window) {
        if constexpr (ender::use_imgui == true) {
            int width = 0;
            int height = 0;
            window->get_client_size(width, height);

            ImGui::SetNextWindowSize({static_cast<float>(width), static_cast<float>(height)});
            ImGui::SetNextWindowPos({0, 0});
            if (ImGui::Begin("ender menu", nullptr,
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize) == true) {
                ImGui::Button("begin");
                ImGui::Button("exit");
                ImGui::End();
            }
        }
    }

    void on_main_window_destroy(ender::engine_window* window) {
    }
}  // namespace game

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    ender::engine_window main_window = {};
    if (main_window.create(game::on_main_window_create, {.title = L"ender",
                                                         .width = 1280,
                                                         .height = 720,
                                                         .on_message_create = nullptr,
                                                         .on_message_destroy = nullptr,
                                                         .instance = instance,
                                                         .cmd_show = cmd_show}) == true) {
        while (main_window.handle_events(game::on_main_window_handle_events) == true) {
            main_window.render_frame(game::on_main_window_render);
        }

        main_window.destroy(game::on_main_window_destroy);

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
