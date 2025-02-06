#pragma once

namespace ender {
#ifndef _DEBUG
    constexpr bool in_debug = false;
#else
    constexpr bool in_debug = true;
#endif  // !_DEBUG

    constexpr bool use_imgui = true;
    constexpr bool use_lua = true;

    constexpr const char* version_string = "0.1";
    constexpr const char* engine_name = "ender";

    class ender_engine {};
}  // namespace ender
