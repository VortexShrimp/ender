#include "lua_window.hpp"

#include <filesystem>

#include "../ender.hpp"
#include "../utils/console.hpp"
#include "../utils/internet.hpp"

bool ender::lua_window::lua_create() {
    debug_print_formatted("--- Lua Start ---\n");

    // Initialize Lua context and bind ender functions.
    m_lua_state.open_libraries(sol::lib::base);

    // Nind ender functions to Lua.s
    lua_bind_core_api();

    if constexpr (use_imgui == true) {
        lua_bind_imgui_api();
    }

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.15f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.92f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.15f, 0.17f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.87f, 0.40f, 1.00f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.33f, 1.00f, 0.67f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.48f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.53f, 1.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.30f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.36f, 0.26f, 0.41f, 0.66f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.53f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.74f, 0.38f, 1.00f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.63f, 0.63f, 0.63f, 1.00f);

    // Load all the script files in the folder specified.
    lua_load_scripts_from_folder("scripts");

    debug_print_formatted("--- Lua End ---\n");

    return true;
}

void ender::lua_window::lua_load_scripts_from_folder(std::string_view folder_name) {
    const auto current_directory = std::filesystem::current_path();
    const auto script_directory = std::filesystem::current_path().append(folder_name);
    debug_print_formatted("[lua] Current path -> '{}'\n[lua] Script path -> '{}'\n",
                          current_directory.string(), script_directory.string());

    int script_count = 0;

    // Create and loop through the script directory.
    std::filesystem::create_directory(script_directory);
    for (auto& entry : std::filesystem::recursive_directory_iterator(script_directory)) {
        // Currently ignore folders.
        if (entry.is_regular_file() == false) {
            continue;
        }

        auto entry_path = entry.path();
        if (entry_path.has_extension() == true) {
            // Load files with "lua" or "ender" extensions.
            if (entry_path.extension().compare(".lua") == 0 ||
                entry_path.extension().compare(".ender") == 0) {
                debug_print_formatted("- Loaded '{}' -> [{} bytes]\n",
                                      entry_path.filename().string(), entry.file_size());
                // Load the script into Lua state.
                m_lua_state.script_file(std::string(folder_name) + std::string("\\") +
                                        entry_path.filename().string());
                script_count += 1;
            }
        }
    }

    debug_print_formatted("Total scripts loaded -> {}\n", script_count);
}

void ender::lua_window::lua_bind_core_api() {
    // Print to the debug console. (if available)
    // debug_print("Hello from Lua!")
    m_lua_state["debug_print"] = debug_print_raw;

    // Does a (blocking) get request.
    // local response = get_request("url.com", "/todos/1")
    m_lua_state["get_request"] = get_request;
}

void ender::lua_window::lua_bind_imgui_api() {
    // ImGui window rendering functions.
    if (use_imgui == true) {
        m_lua_state["imgui_set_next_window_collapsed"] = [](bool collapsed) {
            ImGui::SetNextWindowCollapsed(collapsed);
        };

        // Relative to the screen.
        m_lua_state["imgui_set_next_window_position_absolute"] = [](float x, float y) {
            ImGui::SetNextWindowPos({x, y});
        };

        // Relative to the window.
        m_lua_state["imgui_set_next_window_position_relative"] = [](float x, float y) {
            ImGui::SetNextWindowPos(
                {ImGui::GetMainViewport()->Pos.x + x, ImGui::GetMainViewport()->Pos.y + y});
        };

        m_lua_state["imgui_set_next_window_size"] = [](float x, float y) {
            ImGui::SetNextWindowSize({x, y});
        };

        m_lua_state["imgui_set_next_window_size_and_position"] = [](float posx, float posy,
                                                                    float sizex, float sizey) {
            ImGui::SetNextWindowPos({posx, posy});
            ImGui::SetNextWindowSize({sizex, sizey});
        };

        m_lua_state["imgui_push_font"] = [this](int font_index) {
            ImGui::PushFont(m_imgui_fonts[font_index]);
        };

        m_lua_state["imgui_pop_font"] = ImGui::PopFont;

        // Sets the next window to the size of the client drawing area.
        m_lua_state["imgui_set_next_window_size_to_client_size"] = [this]() {
            const auto [x, y] = get_client_size();
            ImGui::SetNextWindowSize({static_cast<float>(x), static_cast<float>(y)});
        };

        m_lua_state["imgui_add_font_from_file_ttf"] = [this](const char* file_name,
                                                             float size_pixels) {
            m_imgui_fonts.push_back(
                ImGui::GetIO().Fonts->AddFontFromFileTTF(file_name, size_pixels));
        };

        m_lua_state["imgui_set_defualt_font"] = [this](int font_index) {
            ImGui::GetIO().FontDefault = m_imgui_fonts[font_index];
        };

        sol::table window_flags_table = m_lua_state.create_table("imgui_window");
        window_flags_table["no_title"] = 1;
        window_flags_table["no_resize"] = 2;
        window_flags_table["no_move"] = 4;
        window_flags_table["no_collapse"] = 32;

        m_lua_state["imgui_begin_window"] = [](const char* name, int flags) {
            return ImGui::Begin(name, nullptr, flags);
        };

        m_lua_state["imgui_end_window"] = ImGui::End;

        m_lua_state["imgui_button"] = [](const char* label) { return ImGui::Button(label); };
        m_lua_state["imgui_button_size"] = [](const char* label, float x, float y) {
            return ImGui::Button(label, {x, y});
        };

        m_lua_state["imgui_text"] = [](const char* text) { ImGui::Text(text); };
        m_lua_state["imgui_text_centered_x"] = [](const char* text) {
            const ImVec2 window_size = ImGui::GetWindowSize();
            const ImVec2 text_size = ImGui::CalcTextSize(text);

            const float text_x = (window_size.x - text_size.x) * 0.5f;
            ImGui::SetCursorPos({text_x, ImGui::GetCursorPosY()});
            ImGui::Text(text);
        };

        m_lua_state["imgui_separator"] = ImGui::Separator;
        m_lua_state["imgui_spacing"] = ImGui::Spacing;
        m_lua_state["imgui_same_line"] = []() { ImGui::SameLine(); };

        m_lua_state["imgui_show_demo_window"] = []() { ImGui::ShowDemoWindow(); };
    }
}
