/**
 * @file console.hpp
 * @brief Console utility functions and API.
 */

#pragma once
#include <string>
#include <format>
#include <mutex>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../ender.hpp"

namespace ender {
    /**
     * @brief Win32 colors for the console.
     */
    enum class console_color : WORD {
        black = 0,
        dark_blue = FOREGROUND_BLUE,
        dark_green = FOREGROUND_GREEN,
        dark_cyan = FOREGROUND_GREEN | FOREGROUND_BLUE,
        dark_red = FOREGROUND_RED,
        dark_magenta = FOREGROUND_RED | FOREGROUND_BLUE,
        dark_yellow = FOREGROUND_RED | FOREGROUND_GREEN,
        gray = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        dark_gray = FOREGROUND_INTENSITY,
        blue = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
        green = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
        cyan = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
        red = FOREGROUND_INTENSITY | FOREGROUND_RED,
        magenta = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
        yellow = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
        white = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    };

    /**
     * @brief Console class for debug output.
     *
     * @note This class by itself is not thread-safe. Use the helper functions below to print
     * safely.
     */
    class console {
    public:
        console() : m_output(nullptr), m_input(nullptr) {
        }
        ~console();

        /**
         * @brief Allocates and initializes the console.
         * @return True if the console was created successfully.
         */
        bool create();

        /**
         * @brief Free the console and its resources.
         */
        void destroy();

        void set_color(console_color color) noexcept;

        /**
         * @brief  Write std::format text to the output buffer.
         * @tparam ...Args
         * @param format
         * @param ...args
         */
        template <typename... Args>
        inline void print_formatted(std::string_view format, Args&&... args);

        /**
         * @brief Print directly to the console output buffer.
         * @param text Message to print.
         */
        void print_raw(std::string_view text) noexcept;

        /**
         * @brief Change the console window title. Must be called after create().
         * @param new_title
         */
        void set_title(std::string_view new_title);

        /**
         * @brief Get the console window title.
         * @return Converted console title as `std::string`.
         */
        std::string get_title();

        /**
         * @brief Converts wstring to string.
         * @param unicode_text
         * @return
         */
        static std::string unicode_to_multibyte(std::wstring_view unicode_text);
        static std::wstring multibyte_to_unicode(std::string_view multibyte_text);

    private:
        HANDLE m_output;
        HANDLE m_input;
    };

    template <class... Args>
    inline void console::print_formatted(std::string_view format, Args&&... args) {
        const std::string formatted =
            std::vformat(std::string(format), std::make_format_args(std::forward<Args>(args)...));
        print_raw(formatted);
    }

    /**
     * @brief Direct access to the internal debug console.
     * @return Reference to the console singleton instance.
     *
     * @warning Do not use this! It is here to expose the console for helper functions.
     */
    console& debug_console_instance();

    /**
     * @brief Direct access to the internal debug console's mutex.
     * @return Reference to the console's mutex.
     *
     * @warning Do not use this! It is here to expose the console for helper functions.
     */
    std::mutex& debug_console_mutex();

    /**
     * @brief Directly prints to the debug console's buffer during debug builds.
     * @param text Message to print.
     */
    inline void debug_print_raw(std::string_view text) {
        if constexpr (is_debug_build() == true) {
            std::lock_guard lock(debug_console_mutex());
            debug_console_instance().print_raw(text);
        }
    }

    /**
     * @brief Prints formatted text to the debug console's buffer during debug builds.
     * @tparam ...Args
     * @param format
     * @param ...args
     */
    template <class... Args>
    inline void debug_print_formatted(std::string_view format, Args... args) {
        if constexpr (is_debug_build() == true) {
            std::lock_guard lock(debug_console_mutex());
            debug_console_instance().print_formatted(format, args...);
        }
    }
}  // namespace ender
