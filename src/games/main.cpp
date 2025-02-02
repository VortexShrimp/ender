#include <thread>

#ifndef WIN32_LEAN_AND_MEAN
#define WIn32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../engine/platform/window.hpp"

#include <imgui\imgui.h>

namespace game {
    void on_main_window_render() {
        ImGui::ShowDemoWindow();
    }
}  // namespace game

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    ender::engine_window main_window = {};
    if (main_window.create(nullptr, {.title = L"ender",
                                     .width = 1280,
                                     .height = 720,
                                     .instance = instance,
                                     .cmd_show = cmd_show}) == true) {
        while (main_window.is_running() == true) {
            main_window.process_input(nullptr);
            main_window.render_frame(game::on_main_window_render);
        }

        main_window.destroy(nullptr);

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
