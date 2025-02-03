#include <thread>

#include "../engine/ender.hpp"
#include "../engine/platform/console.hpp"
#include "../engine/platform/window.hpp"

#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>

namespace game {
    class example {
    public:
        enum class screens { start, in_game, end };

        void set_current_screen(screens new_screen) noexcept {
            m_current_screen = new_screen;
        }

        screens get_current_screen() const noexcept {
            return m_current_screen;
        }

        ender::console& console() noexcept {
            return m_console;
        }

    private:
        screens m_current_screen;
        ender::console m_console;
    };

    bool example_window_create(example* game, ender::game_window* window) {
        if (ender::use_imgui == true) {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 32.0f);
            io.Fonts->Build();

            ImGui_ImplDX11_InvalidateDeviceObjects();
            ImGui_ImplDX11_CreateDeviceObjects();

            io.FontDefault = io.Fonts->Fonts[0];
        }

        // Setup game stuff.
        game->set_current_screen(game::example::screens::start);

        // Create our console.
        if (game->console().create() == false) {
            return false;
        }

        game->console().set_title(L"example debug console");
        game->console().write(L"Game initialized sucessfully.");

        return true;
    }

    bool handle_events(example* game, ender::game_window* window) {
        // Return false to exit the game.
        if (window->is_running() == false) {
            return false;
        }

        return true;
    }

    void render_frame(example* game, ender::game_window* window) {
        if constexpr (ender::use_imgui == true) {
            switch (game->get_current_screen()) {
                case game::example::screens::start: {
                    const ender::vec2i client_size = window->get_client_size();
                    // Set the next window to the size of the client draw area.
                    ImGui::SetNextWindowSize(ImVec2{static_cast<float>(client_size.x),
                                                    static_cast<float>(client_size.y)});
                    ImGui::SetNextWindowPos({0, 0});
                    if (ImGui::Begin("ender menu", nullptr,
                                     ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar |
                                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize) ==
                        true) {
                        ImGui::Text("ender example");
                        if (ImGui::Button("begin", {250, 100}) == true) {
                            game->set_current_screen(game::example::screens::in_game);
                        }
                        ImGui::Text("%f", window->get_delta_time());
                        if (ImGui::Button("exit", {250, 100}) == true) {
                            window->stop_running();
                        }
                        ImGui::End();
                    }
                    break;
                }
                case game::example::screens::in_game:
                    ImGui::GetForegroundDrawList()->AddRectFilled({20, 20}, {300, 300},
                                                                  ImColor(255, 0, 0, 255));
                    break;
                case game::example::screens::end:
                    break;
            }
        }
    }

    void window_destroy(example* game, ender::game_window* window) {
        game->console().destroy();
    }
}  // namespace game

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    game::example game = {};
    ender::game_window main_window = {};
    if (main_window.create(
            &game,
            reinterpret_cast<ender::game_window::create_function>(game::example_window_create),
            {.title = L"ender",
             .width = 1280,
             .height = 720,
             .on_message_create = nullptr,
             .on_message_destroy = nullptr,
             .instance = instance,
             .cmd_show = cmd_show}) == true) {
        while (main_window.handle_events(
                   &game, reinterpret_cast<ender::game_window::handle_events_function>(
                              game::handle_events)) == true) {
            main_window.render_frame(
                &game,
                reinterpret_cast<ender::game_window::render_frame_function>(game::render_frame));
        }

        main_window.destroy(
            &game, reinterpret_cast<ender::game_window::destroy_function>(game::window_destroy));

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
