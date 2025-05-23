#include "internet.hpp"

#include "console.hpp"

ender::internet::~internet() {
    destroy();
}

bool ender::internet::create(std::string_view user_agent) {
    m_internet = InternetOpen(multibyte_to_unicode(user_agent.data()).c_str(),
                              INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (m_internet == INVALID_HANDLE_VALUE) {
        return false;
    }

    return true;
}

void ender::internet::destroy() {
    if (m_internet != nullptr) {
        InternetCloseHandle(m_internet);
        m_internet = nullptr;
    }
}

bool ender::internet::get(std::string_view url, std::string_view objects,
                          std::string& response_out) {
    timer_basic request_timer = {};

    const HINTERNET connect = InternetConnectA(m_internet, url.data(), INTERNET_DEFAULT_HTTP_PORT,
                                               NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
    if (connect == 0) {
        return false;
    }

    const HINTERNET request =
        HttpOpenRequestA(connect, "GET", objects.data(), NULL, NULL, NULL, 0, 0);
    if (request == 0) {
        InternetCloseHandle(connect);
        debug_print("Failed to Open Request. Last Error -> {}\n", GetLastError());
        return false;
    }

    const BOOL was_request_sent = HttpSendRequestA(request, NULL, 0, NULL, 0);
    if (was_request_sent == FALSE) {
        InternetCloseHandle(request);
        InternetCloseHandle(connect);
        debug_print("Failed to Send Request. Last Error -> {}\n", GetLastError());
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

    debug_print("[http] Get request to '{}' completed in {} seconds.\n", url,
                request_timer.time_elapsed_seconds());

    return true;
}

bool ender::internet::post(std::string_view url, std::string_view objects, std::string_view data,
                           std::string& response_out) {
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
        HttpSendRequestA(request, NULL, 0, (LPVOID)data.data(),
                         static_cast<DWORD>(data.size()));  // Ugly LPVOID cast.
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

HINTERNET ender::internet::internet_handle() const {
    return m_internet;
}

std::string ender::get_request(std::string_view url, std::string_view objects) {
    std::string response = "";

    internet client = {};
    if (client.create() == true) {
        client.get(url, objects, response);
        client.destroy();
    }

    return response;
}

void ender::post_request(std::string_view url, std::string_view objects, std::string_view data) {
    internet client = {};
    if (client.create() == true) {
        std::string response = "";
        client.post(url, objects, data, response);
        client.destroy();
    }
}

void ender::get_request_callback(std::string_view url, std::string_view objects,
                                 std::function<void(std::string)> callback) {
    internet client = {};
    if (client.create() == true) {
        std::string response = "";
        if (client.get(url, objects, response) == true) {
            if (response.empty() == false) {
                callback(response);
            }
        }
        client.destroy();
    }
}