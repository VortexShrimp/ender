#pragma once
#include <string>
#include <vector>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>

#include "timer.hpp"

namespace ender {
    /**
     * @brief Opens a connection to a url for simple get/post requests.
     *
     * TODO:
     * - Add async get & post.
     *
     */
    class internet_client {
    public:
        internet_client() : m_internet(INVALID_HANDLE_VALUE) {
        }
        ~internet_client();

        bool create(std::wstring_view user_agent = L"WinHttpClient/1.0");
        void destroy();

        /**
         * @brief Make a (blocking) get request to a url.
         *
         * @example client.get("jsonplaceholder.typicode.com", "/todos/1", response)
         *
         * @param url
         * @param objects
         * @param downloaded
         * @return
         */
        bool get(std::string_view url, std::string_view objects, std::string& response_out);
        bool post(std::string_view url, std::string_view objects, std::string_view data,
                  std::string response_out);

    private:
        HINTERNET m_internet;
    };
}  // namespace ender
