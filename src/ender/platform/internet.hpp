#pragma once
#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>

namespace ender {
    class internet_client {
    public:
        void create(std::wstring_view user_agent = L"WinHttpClient/1.0");
        void destroy();

        bool get(std::wstring_view url, std::wstring& response);
        bool post(std::wstring_view url, const std::wstring& data, std::wstring& response);

        void add_header(const std::wstring& header);

    private:
        bool send_request(std::wstring_view url, const std::wstring* postData,
                          std::wstring& response);

        HINTERNET m_internet;
        std::vector<std::wstring> m_headers;
    };
}  // namespace ender
