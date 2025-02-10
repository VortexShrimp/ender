#include "console.hpp"

ender::console::~console() {
    destroy();
}

bool ender::console::create() {
    if (AllocConsole() == FALSE) {
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

void ender::console::print_raw(std::string_view text) noexcept {
    if (m_output == INVALID_HANDLE_VALUE) {
        return;
    }

    std::wstring converted = multibyte_to_unicode(text);

    DWORD written;
    WriteConsole(m_output, converted.data(), static_cast<DWORD>(converted.length()), &written,
                 NULL);
}

void ender::console::set_window_size(vec2i new_size) {
    SMALL_RECT rect = {0, 0, static_cast<SHORT>(new_size.x - 1),
                       static_cast<SHORT>(new_size.y - 1)};
    SetConsoleWindowInfo(m_output, TRUE, &rect);
}

void ender::console::set_title(std::string_view new_title) {
    SetConsoleTitle(multibyte_to_unicode(new_title).c_str());
}

std::wstring_view ender::console::get_title() {
    wchar_t buffer[256];
    GetConsoleTitle(buffer, 256);
    return buffer;
}

std::string ender::console::unicode_to_multibyte(std::wstring_view unicode_text) {
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

std::wstring ender::console::multibyte_to_unicode(std::string_view multibyte_text) {
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

auto ender::get_debug_console() -> console* {
    static ender::console s_debug_console;

    static bool once = true;
    if (once == true) {
        s_debug_console = {};
        s_debug_console.create();
        s_debug_console.set_title("debug console");

        once = false;
    }

    return &s_debug_console;
}

std::mutex& ender::get_debug_console_mutex() {
    static std::mutex s_console_mutex;
    return s_console_mutex;
}

void ender::debug_print_raw(std::string_view text) {
    if constexpr (in_debug == true) {
        std::lock_guard lock(get_debug_console_mutex());
        get_debug_console()->print_raw(text);
    }
}
