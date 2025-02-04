#include "internet.hpp"

ender::platform_internet::~platform_internet() {
    destroy();
}

bool ender::platform_internet::create(std::wstring_view user_agent) {
    m_internet = InternetOpen(user_agent.data(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (m_internet == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

void ender::platform_internet::destroy() {
    if (m_internet != nullptr) {
        InternetCloseHandle(m_internet);
        m_internet = nullptr;
    }
}

bool ender::platform_internet::get(std::string_view url, std::string_view objects,
                                   std::string& response) {
    const HINTERNET connect = InternetConnectA(m_internet, url.data(), INTERNET_DEFAULT_HTTP_PORT,
                                               NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (connect == INVALID_HANDLE_VALUE) {
        return false;
    }

    const HINTERNET request =
        HttpOpenRequestA(connect, "GET", objects.data(), NULL, NULL, NULL, 0, 0);
    if (request == INVALID_HANDLE_VALUE) {
        InternetCloseHandle(connect);
        return false;
    }

    const BOOL was_request_sent = HttpSendRequestA(request, NULL, 0, NULL, 0);
    if (was_request_sent == FALSE) {
        const int last_error = GetLastError();
        InternetCloseHandle(request);
        InternetCloseHandle(connect);
        return false;
    }

    char buffer[4096];
    DWORD bytes_read;
    BOOL is_read;

    do {
        is_read = InternetReadFile(request, buffer, sizeof(buffer), &bytes_read);
        if (is_read == TRUE && bytes_read != 0) {
            response.append(buffer, bytes_read);
        } else if (is_read == false) {
            break;
        }
    } while (is_read == TRUE && bytes_read != 0);

    InternetCloseHandle(request);
    InternetCloseHandle(connect);

    return true;
}
