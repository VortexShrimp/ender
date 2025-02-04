#include "internet.hpp"

void ender::internet_client::create(std::wstring_view user_agent) {
    m_internet = InternetOpen(user_agent.data(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
}

void ender::internet_client::destroy() {
    if (m_internet != nullptr) {
        InternetCloseHandle(m_internet);
    }
}

bool ender::internet_client::get(std::wstring_view url, std::wstring& response) {
    return send_request(url, nullptr, response);
}

bool ender::internet_client::post(std::wstring_view url, const std::wstring& data,
                                  std::wstring& response) {
    return send_request(url, &data, response);
}

void ender::internet_client::add_header(const std::wstring& header) {
    m_headers.emplace_back(header);
}

bool ender::internet_client::send_request(std::wstring_view url, const std::wstring* postData,
                                          std::wstring& response) {
    HINTERNET connection =
        InternetOpenUrl(m_internet, url.data(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (connection == nullptr) {
        return false;
    }

    wchar_t buffer[4096];
    DWORD bytes_read;
    while (InternetReadFile(connection, buffer, sizeof(buffer) - 1, &bytes_read) == TRUE &&
           bytes_read > 0) {
        buffer[bytes_read] = '\0';
        response.append(buffer);
    }

    InternetCloseHandle(connection);
    return true;
}
