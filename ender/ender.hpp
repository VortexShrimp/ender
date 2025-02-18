#pragma once

// Use ImGui in builds.
#define ENDER_IMGUI

namespace ender {
    constexpr const char* version_name = "ender 0.1.0";

    /**
     * @brief Check if the current build is a debug build.
     * @return True if the build is a debug build.
     */
    consteval bool is_debug_build() {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif  // _DEBUG
    }

    /**
     * @brief Display a dialogue box with an error message.
     * @param message
     */
    void error_message_box(const char* message);
}  // namespace ender
