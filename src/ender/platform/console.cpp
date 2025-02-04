#include "console.hpp"

bool ender::platform_console::create() {
    if (AllocConsole() == FALSE) {
        return false;
    }

    m_output = GetStdHandle(STD_OUTPUT_HANDLE);
    m_input = GetStdHandle(STD_INPUT_HANDLE);

    if (m_output == INVALID_HANDLE_VALUE || m_input == INVALID_HANDLE_VALUE) {
        FreeConsole();
        return false;
    }

    if (GetConsoleScreenBufferInfo(m_output, &m_csbi) == FALSE) {
        FreeConsole();
        return false;
    }

    m_width = m_csbi.srWindow.Right - m_csbi.srWindow.Left + 1;
    m_height = m_csbi.srWindow.Bottom - m_csbi.srWindow.Top + 1;

    return true;
}

void ender::platform_console::destroy() {
    FreeConsole();

    m_output = INVALID_HANDLE_VALUE;
    m_input = INVALID_HANDLE_VALUE;
}

void ender::platform_console::write_raw(std::wstring_view text) {
    if (m_output == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD written;
    WriteConsole(m_output, text.data(), text.length(), &written, NULL);
}

void ender::platform_console::set_window_size(vec2i new_size) {
    SMALL_RECT rect = {0, 0, static_cast<SHORT>(new_size.x - 1),
                       static_cast<SHORT>(new_size.y - 1)};
    SetConsoleWindowInfo(m_output, TRUE, &rect);
}

void ender::platform_console::set_title(std::string_view new_title) {
    SetConsoleTitle(multibyte_to_unicode(new_title).c_str());
}

std::wstring_view ender::platform_console::get_title() {
    wchar_t buffer[256];
    GetConsoleTitle(buffer, 256);
    return buffer;
}

std::string ender::platform_console::unicode_to_multibyte(std::wstring_view unicode_text) {
    if (unicode_text.empty() == true) {
        return {};
    }

    const int size = WideCharToMultiByte(CP_UTF8, 0, unicode_text.data(), unicode_text.length(),
                                         nullptr, 0, nullptr, nullptr);
    if (size == 0) {
        return {};
    }

    std::string converted_text(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, unicode_text.data(), unicode_text.length(),
                        converted_text.data(), size, nullptr, nullptr);

    return converted_text;
}

std::wstring ender::platform_console::multibyte_to_unicode(std::string_view multibyte_text) {
    if (multibyte_text.empty() == true) {
        return {};
    }

    const int size = MultiByteToWideChar(CP_UTF8, 0, multibyte_text.data(),
                                         static_cast<int>(multibyte_text.length()), nullptr, 0);
    if (size == 0) {
        return {};
    }

    std::wstring converted_text(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, multibyte_text.data(),
                        static_cast<int>(multibyte_text.length()), converted_text.data(), size);

    return converted_text;
}
