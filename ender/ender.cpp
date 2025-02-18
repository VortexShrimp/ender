#include "ender.hpp"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "utils\console.hpp"

namespace ender {
    void error_message_box(const char* message) {
        // Print the message to the debug console before the message box.
        debug_print_formatted("Exception: {}\n", message);

        // Create the caption based on ender::version_name.
        const std::wstring caption_wide =
            console::multibyte_to_unicode(std::format("{} error", version_name));

        // Convert the message to wide-string.
        const std::wstring message_wide = console::multibyte_to_unicode(message);

        MessageBox(nullptr, message_wide.c_str(), caption_wide.c_str(), MB_ICONERROR);
    }
}  // namespace ender