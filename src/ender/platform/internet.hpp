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
        bool create(std::wstring_view user_agent = L"WinHttpClient/1.0");
        void destroy();

    private:
        HINTERNET m_internet;
    };
}  // namespace ender
