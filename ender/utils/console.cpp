#include "console.hpp"

ender::console::~console() {
    destroy();
}

bool ender::console::create() {
    if (AllocConsole() == FALSE) {
        error_message_box("Failed to allocate console.");
        return false;
    }

    m_output = GetStdHandle(STD_OUTPUT_HANDLE);
    m_input = GetStdHandle(STD_INPUT_HANDLE);

    if (m_output == INVALID_HANDLE_VALUE || m_input == INVALID_HANDLE_VALUE) {
        FreeConsole();
        return false;
    }

    return true;
}

void ender::console::destroy() {
    FreeConsole();

    m_output = INVALID_HANDLE_VALUE;
    m_input = INVALID_HANDLE_VALUE;
}

void ender::console::set_title(std::string_view new_title) {
    SetConsoleTitle(multibyte_to_unicode(new_title).c_str());
}

std::string ender::console::get_title() {
    wchar_t buffer[256];
    if (GetConsoleTitle(buffer, 256) == FALSE) {
        error_message_box("Failed to get console title.");
        return {};
    }

    return std::string{unicode_to_multibyte(buffer)};
}

void ender::console::set_color(console_color color) noexcept {
    if (m_output == INVALID_HANDLE_VALUE) {
        error_message_box("Console not initialized.");
        return;
    }

    if (SetConsoleTextAttribute(m_output, static_cast<WORD>(color)) == FALSE) {
        error_message_box("Failed to set console color.");
        return;
    }
}

void ender::console::print_raw(std::string_view text) noexcept {
    if (m_output == INVALID_HANDLE_VALUE) {
        return;
    }

    // Lock the mutex to ensure thread safety while printing.
    std::lock_guard lock(m_mutex);

    const std::wstring converted = multibyte_to_unicode(text);

    DWORD written;
    WriteConsole(m_output, converted.data(), static_cast<DWORD>(converted.length()), &written,
                 NULL);
}

std::string ender::unicode_to_multibyte(std::wstring_view unicode_text) {
    if (unicode_text.empty() == true) {
        return {};
    }

    const int size =
        WideCharToMultiByte(CP_UTF8, 0, unicode_text.data(),
                            static_cast<int>(unicode_text.length()), nullptr, 0, nullptr, nullptr);
    if (size == 0) {
        return {};
    }

    std::string converted_text(size, '\0');
    WideCharToMultiByte(CP_UTF8, 0, unicode_text.data(), static_cast<int>(unicode_text.length()),
                        converted_text.data(), size, nullptr, nullptr);

    return converted_text;
}

std::wstring ender::multibyte_to_unicode(std::string_view multibyte_text) {
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