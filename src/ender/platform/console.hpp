#pragma once
#include <string_view>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ender {
    class console {
    public:
        console() : m_console(nullptr), m_input(nullptr), m_csbi({}), m_width(0), m_height(0) {
        }

        bool create();
        void destroy();

        void write(std::wstring_view text);

        void set_title(std::wstring_view new_title);

    private:
        HANDLE m_console;
        HANDLE m_input;

        CONSOLE_SCREEN_BUFFER_INFO m_csbi;

        int m_width;
        int m_height;
    };
}  // namespace ender
