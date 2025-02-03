#include "console.hpp"

bool ender::console::create() {
    if (AllocConsole() == FALSE) {
        return false;
    }

    m_console = GetStdHandle(STD_OUTPUT_HANDLE);
    m_input = GetStdHandle(STD_INPUT_HANDLE);

    if (GetConsoleScreenBufferInfo(m_console, &m_csbi) == FALSE) {
        return false;
    }

    m_width = m_csbi.srWindow.Right - m_csbi.srWindow.Left + 1;
    m_height = m_csbi.srWindow.Bottom - m_csbi.srWindow.Top + 1;

    return true;
}

void ender::console::destroy() {
    FreeConsole();
}

void ender::console::write(std::wstring_view text) {
    DWORD written;
    WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), text.data(), text.length(), &written, NULL);
}

void ender::console::set_title(std::wstring_view new_title) {
    SetConsoleTitle(new_title.data());
}
