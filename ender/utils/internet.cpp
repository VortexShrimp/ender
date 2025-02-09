#include "internet.hpp"

#include "console.hpp"

ender::internet_client::~internet_client() {
    destroy();
}

bool ender::internet_client::create(std::wstring_view user_agent) {
    m_internet = InternetOpen(user_agent.data(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (m_internet == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

void ender::internet_client::destroy() {
    if (m_internet != nullptr) {
        InternetCloseHandle(m_internet);
        m_internet = nullptr;
    }
}

bool ender::internet_client::get(std::string_view url, std::string_view objects,
                                 std::string& response) {
    debug_print_formatted("--- HTTP GET START ---\n");
    debug_print_formatted("url -> '{}'\n", url);

    high_resolution_timer timer = {};

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
        } else if (is_read == FALSE) {
            break;
        }
    } while (is_read == TRUE && bytes_read != 0);

    InternetCloseHandle(request);
    InternetCloseHandle(connect);

    debug_print_formatted("Completed in {} seconds.\n", timer.get_elapsed_time_seconds());
    debug_print_formatted("--- HTTP GET END ---\n");

    return true;
}

bool ender::internet_client::post(std::string_view url, std::string_view objects,
                                  std::string_view data, std::string response_out) {
    const HINTERNET connect = InternetConnectA(m_internet, url.data(), INTERNET_DEFAULT_HTTP_PORT,
                                               NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (connect == INVALID_HANDLE_VALUE) {
        return false;
    }

    const HINTERNET request =
        HttpOpenRequestA(connect, "POST", objects.data(), NULL, NULL, NULL, 0, 0);
    if (request == INVALID_HANDLE_VALUE) {
        InternetCloseHandle(connect);
        return false;
    }

    const BOOL was_request_sent =
        HttpSendRequestA(request, NULL, 0, (LPVOID)data.data(), data.size());  // Ugly LPVOID cast.
    if (was_request_sent == FALSE) {
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
            response_out.append(buffer, bytes_read);
        } else if (is_read == FALSE) {
            break;
        }
    } while (is_read == TRUE && bytes_read != 0);

    InternetCloseHandle(request);
    InternetCloseHandle(connect);

    return true;
}
