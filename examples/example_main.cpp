#include <thread>

#include "../src/ender/ender.hpp"
#include "../src/ender/platform/console.hpp"
#include "../src/ender/platform/window.hpp"

#include <imgui\imgui.h>
#include <imgui\imgui_impl_dx11.h>

namespace game {
    /**
     * @brief Example game window with a menu.
     *
     * Create your 'game' class by inhereting from ender::game_window.
     *
     */
    class example_game_window : public ender::game_window {
    public:
        enum class screens { start, in_game, end };

        bool on_create() {
            if (ender::use_imgui == true) {
                ImGuiIO& io = ImGui::GetIO();
                io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/bahnschrift.ttf", 32.0f);
                io.Fonts->Build();

                ImGui_ImplDX11_InvalidateDeviceObjects();
                ImGui_ImplDX11_CreateDeviceObjects();

                io.FontDefault = io.Fonts->Fonts[0];
            }

            // Create our console.
            if (m_console.create() == false) {
                return false;
            }

            // Setup game stuff.
            m_current_screen = screens::start;

            m_console.set_title(L"example debug console");
            m_console.write(L"Game initialized sucessfully.");

            return true;
        }

        void on_destroy() {
            m_console.destroy();
        }

        bool on_handle_events() {
            if (m_is_running == false) {
                return false;
            }

            // Handle game logic and processing.

            return true;
        }

        void on_render_frame() {
            if constexpr (ender::use_imgui == true) {
                switch (m_current_screen) {
                    case screens::start: {
                        const ender::vec2i client_size = get_client_size();
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
                                m_current_screen = screens::in_game;
                            }
                            ImGui::Text("%f", get_delta_time());
                            if (ImGui::Button("exit", {250, 100}) == true) {
                                m_is_running = false;
                            }
                            ImGui::End();
                        }
                        break;
                    }
                    case screens::in_game:
                        ImGui::GetForegroundDrawList()->AddRectFilled({20, 20}, {300, 300},
                                                                      ImColor(255, 0, 0, 255));
                        break;
                    case screens::end:
                        break;
                }
            }
        }

    private:
        screens m_current_screen;

        ender::console m_console;
    };

    //
    // Window event handlers:
    //
    // You can either do your logic in these through the ctx pointer.
    // Or you can just pass it on and do your logic in the game's class
    // like I've done here.
    //

    bool example_game_window_create(ender::game_window* ctx) {
        auto game = static_cast<example_game_window*>(ctx);
        return game->on_create();
    }

    bool example_game_window_handle_events(ender::game_window* ctx) {
        auto game = static_cast<example_game_window*>(ctx);
        return game->on_handle_events();
    }

    void example_game_window_render_frame(ender::game_window* ctx) {
        auto game = static_cast<example_game_window*>(ctx);
        game->on_render_frame();
    }

    void example_game_window_destroy(ender::game_window* ctx) {
        auto game = static_cast<example_game_window*>(ctx);
        game->on_destroy();
    }
}  // namespace game

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR cmd_line, INT cmd_show) {
    game::example_game_window example_window;
    if (example_window.create(game::example_game_window_create, {.title = L"ender example",
                                                                 .width = 1280,
                                                                 .height = 720,
                                                                 .on_message_create = nullptr,
                                                                 .on_message_destroy = nullptr,
                                                                 .instance = instance,
                                                                 .cmd_show = cmd_show}) == true) {
        while (example_window.handle_events(game::example_game_window_handle_events) == true) {
            example_window.render_frame(game::example_game_window_render_frame);
        }
        example_window.destroy(game::example_game_window_destroy);

        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}
