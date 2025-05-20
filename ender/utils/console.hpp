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
#include "timer.hpp"

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
     * @brief Console singleton class for logging messages.
     */
    class console {
    public:
        console(const console&) = delete;
        console& operator=(const console&) = delete;
        console(console&&) = delete;
        console& operator=(console&&) = delete;

        /**
         * @brief Access the singleton instance of this class.
         * @return
         */
        static console& instance() noexcept;

        /**
         * @brief Print directly to the console output buffer.
         * @param text Message to print.
         */
        void print_raw(std::string_view text) noexcept;

        /**
         * @brief  Write std::format text to the output buffer.
         * @tparam ...Args
         * @param format
         * @param ...args
         */
        template <typename... Args>
        inline void print_formatted(std::string_view format, Args&&... args);

    private:
        console() : m_output(nullptr), m_input(nullptr), m_timer() {
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
         * @brief Set the console text color.
         * @param color New color to set.
         */
        void set_color(console_color color) noexcept;

        HANDLE m_output;
        HANDLE m_input;

        std::mutex m_mutex;
        timer_basic m_timer;
    };

    template <class... Args>
    inline void console::print_formatted(std::string_view format, Args&&... args) {
        const std::string formatted =
            std::vformat(std::string(format), std::make_format_args(std::forward<Args>(args)...));
        print_raw(formatted);
    }

    /**
     * @brief Prints formatted text to the debug console's buffer during debug builds.
     * @tparam ...Args
     * @param format
     * @param ...args
     */
    template <class... Args>
    inline void debug_print(std::string_view format, Args... args) {
        if constexpr (is_debug_build() == true) {
            console::instance().print_formatted(format, args...);
        }
    }

    inline console& console::instance() noexcept {
        static console instance;

        if (static bool once = true; once == true) {
            instance.create();
            instance.set_title("Ender Debug Console");
            once = false;
        }

        return instance;
    }

    std::string unicode_to_multibyte(std::wstring_view unicode_text);
    std::wstring multibyte_to_unicode(std::string_view multibyte_text);
}  // namespace ender
