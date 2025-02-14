#pragma once

namespace ender {
#ifndef _DEBUG
    constexpr bool in_debug = false;
#else
    constexpr bool in_debug = true;
#endif  // !_DEBUG

    // Only supports 1 window at a time.
    constexpr bool use_imgui = true;
    constexpr bool use_standalone_renderer = false;

    constexpr bool use_lua = true;

    constexpr const char* version_string = "0.1";
    constexpr const char* engine_name = "ender";
}  // namespace ender
