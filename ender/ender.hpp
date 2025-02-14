#pragma once

// Use ImGui in builds.
#define ENDER_IMGUI

namespace ender {
    consteval bool is_debug_build() {
#ifdef _DEBUG
        return true;
#else
        return false;
#endif  // _DEBUG
    }

    // Currently unused.
    constexpr bool use_lua = true;
}  // namespace ender
