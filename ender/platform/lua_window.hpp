#pragma once
#include "window.hpp"

#include "../external/sol/sol.hpp"

namespace ender {
    /**
     * @brief Create a window with Lua state attached to it.
     * @todo Move this?
     */
    class lua_window : public window {
    public:
        lua_window() : window() {
        }

    protected:
        /**
         * @brief Initializes Lua state and binds important functions.
         * @return
         */
        bool lua_create();

        /**
         * @brief Load all the scripts in a specific folder.
         * @param folder_name Name of folder relative to the .exe file.
         */
        void lua_load_scripts_from_folder(std::string_view folder_name);

        sol::state m_lua_state;

    private:
        void lua_bind_functions();
        void lua_bind_imgui_functions();
    };
}  // namespace ender
