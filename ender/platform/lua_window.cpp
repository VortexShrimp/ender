#include "lua_window.hpp"

#include <filesystem>

#include "../ender.hpp"
#include "../utils/console.hpp"

bool ender::lua_window::lua_create() {
    debug_print_formatted("--- Lua Start ---\n");

    // Initialize Lua context and bind ender functions.
    m_lua_state.open_libraries(sol::lib::base);

    // Nind ender functions to Lua.s
    lua_bind_functions();

    if constexpr (use_imgui == true) {
        lua_bind_imgui_functions();
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

void ender::lua_window::lua_bind_functions() {
    m_lua_state["debug_print_raw"] = debug_print_raw;
}

void ender::lua_window::lua_bind_imgui_functions() {
    // ImGui window rendering functions.
    if (use_imgui == true) {
        m_lua_state["imgui_begin_window"] = [](const char* name) { return ImGui::Begin(name); };
        m_lua_state["imgui_end_window"] = []() { ImGui::End(); };
        m_lua_state["imgui_text"] = [](const char* text) { ImGui::Text(text); };
        m_lua_state["imgui_button"] = [](const char* label) { return ImGui::Button(label); };

        m_lua_state["imgui_show_demo_window"] = []() { ImGui::ShowDemoWindow(); };
    }
}
