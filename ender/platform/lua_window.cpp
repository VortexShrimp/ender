#include "lua_window.hpp"

#include <filesystem>

#include "../ender.hpp"
#include "../utils/console.hpp"

bool ender::lua_window::lua_create() {
    debug_print_formatted("--- Lua Start ---\n");

    // Initialize Lua context and bind ender functions.
    m_lua_state.open_libraries(sol::lib::base);

    // Nind ender functions to Lua.s
    lua_bind_core_api();

    if constexpr (use_imgui == true) {
        lua_bind_imgui_api();
    }

    // Load all the script files in the folder specified.
    lua_load_scripts_from_folder("scripts");

    debug_print_formatted("--- Lua End ---\n");

    return true;
}

void ender::lua_window::lua_load_scripts_from_folder(std::string_view folder_name) {
    auto current_directory = std::filesystem::current_path();
    auto script_directory = std::filesystem::current_path().append(folder_name);
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
    m_lua_state["debug_print_raw"] = debug_print_raw;
}

void ender::lua_window::lua_bind_imgui_api() {
    // ImGui window rendering functions.
    if (use_imgui == true) {
        m_lua_state["imgui_set_next_window_collapsed"] = [](bool collapsed) {
            ImGui::SetNextWindowCollapsed(collapsed);
        };
        m_lua_state["imgui_set_next_window_position"] = [](float x, float y) {
            ImGui::SetNextWindowPos({x, y});
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
        m_lua_state["imgui_pop_font"] = []() { ImGui::PopFont(); };

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

        // ImGuiWindowFlags_NoMove = 4
        // ImGuiWindowFlags_NoCollapse = 32
        // ImGuiWindowFlags_NoResize = 2
        // ImGuiWindowFlags_NoTitleBar = 1
        m_lua_state["imgui_begin_window"] = [](const char* name, int flags) {
            return ImGui::Begin(name, nullptr, flags);
        };
        m_lua_state["imgui_end_window"] = []() { ImGui::End(); };
        m_lua_state["imgui_text"] = [](const char* text) { ImGui::Text(text); };
        m_lua_state["imgui_button"] = [](const char* label) { return ImGui::Button(label); };

        m_lua_state["imgui_separator"] = []() { ImGui::Separator(); };
        m_lua_state["imgui_spacing"] = []() { ImGui::Spacing(); };

        m_lua_state["imgui_show_demo_window"] = []() { ImGui::ShowDemoWindow(); };
    }
}
