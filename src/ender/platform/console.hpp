#pragma once
#include <string>
#include <format>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../math/vectors.hpp"

namespace ender {
    class console {
    public:
        console() : m_output(nullptr), m_input(nullptr), m_csbi({}), m_width(0), m_height(0) {
        }

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
        void write(std::string_view format, Args&&... args);

        void set_window_size(vec2i new_size);

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

    private:
        void write_raw(std::wstring_view text);

        HANDLE m_output;
        HANDLE m_input;

        CONSOLE_SCREEN_BUFFER_INFO m_csbi;

        int m_width;
        int m_height;
    };

    template <typename... Args>
    inline void console::write(std::string_view format, Args&&... args) {
        std::string formatted =
            std::vformat(std::string(format), std::make_format_args(std::forward<Args>(args)...));
        write_raw(multibyte_to_unicode(formatted));
    }
}  // namespace ender
