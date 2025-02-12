#pragma once
#include <string>
#include <format>
#include <mutex>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../ender.hpp"
#include "../math/vectors.hpp"

namespace ender {
    class console {
    public:
        console() : m_output(nullptr), m_input(nullptr) {
        }
        ~console();

        /**
         * @brief Allocates and initializes the console.
         * @return
         */
        bool create();
        void destroy();

        /**
         * @brief  Write std::format text to the output buffer.
         * @tparam ...Args
         * @param format
         * @param ...args
         */
        template <typename... Args>
        void print_formatted(std::string_view format, Args&&... args);

        /**
         * @brief Change the console window title. Must be called after create().
         * @param new_title
         */
        void set_title(std::string_view new_title);
        std::wstring_view get_title();

        /**
         * @brief Converts wstring to string.
         * @param unicode_text
         * @return
         */
        static std::string unicode_to_multibyte(std::wstring_view unicode_text);
        static std::wstring multibyte_to_unicode(std::string_view multibyte_text);

        /**
         * @brief Print directly to the console output buffer.
         *
         * Multibyte text is converted to Unicode just before its printed.
         *
         * @param text
         */
        void print_raw(std::string_view text) noexcept;

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
     * @brief Access the debug console singleton.
     * The console is created the fist time this function is called.
     *
     * @warning You shouldn't use this. It's here to expose the console for debug_print_formatted.
     *
     * @return Pointer to internal console class.
     */
    console& debug_console();

    /**
     * @brief Access the internal debug console's mutex.
     *
     * @warning You shouldn't use this. It's here to expose the console for debug_print_formatted.
     *
     * @return
     */
    std::mutex& debug_console_mutex();

    /**
     * @brief Directly prints to the debug console's buffer. Useful for lua binding.
     * @param text
     */
    inline void debug_print_raw(std::string_view text) {
        {
            if constexpr (in_debug == true) {
                std::lock_guard lock(debug_console_mutex());
                debug_console().print_raw(text);
            }
        }
    }

    /**
     * @brief Print to the debug console with formatting.
     *
     * If compiled, creates a console on first use. Does not compile in debug mode.
     *
     * @todo Pass rvalues instead of copies?
     * `Args...` -> `Args&&...`.
     *
     * @tparam ...Args
     * @param format
     * @param ...args
     */
    template <class... Args>
    inline void debug_print_formatted(std::string_view format, Args... args) {
        if constexpr (in_debug == true) {
            std::lock_guard lock(debug_console_mutex());
            debug_console().print_formatted(format, args...);
        }
    }
}  // namespace ender
