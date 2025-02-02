#pragma once

namespace ender {
#ifndef _DEBUG
    constexpr bool in_debug = false;
#else
    constexpr bool in_debug = true;
#endif  // !_DEBUG
    constexpr const char* version_string = "0.1";
    constexpr const char* engine_name = "ender";
}  // namespace ender
