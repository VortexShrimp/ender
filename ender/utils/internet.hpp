#pragma once
#include <string>
#include <functional>

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
    class internet {
    public:
        internet() : m_internet(INVALID_HANDLE_VALUE) {
        }
        ~internet();

        bool create(std::string_view user_agent = "WinHttpClient/1.0");
        void destroy();

        /**
         * @brief Make a get request to a url.
         *
         * @param url
         * @param objects
         * @param downloaded
         * @return
         */
        bool get(std::string_view url, std::string_view objects, std::string& response_out);
        bool post(std::string_view url, std::string_view objects, std::string_view data,
                  std::string& response_out);

        HINTERNET internet_handle() const;

    private:
        HINTERNET m_internet;
    };

    /**
     * @brief Get request to url.
     * @param url
     * @param objects
     * @return The API response as std::string.
     */
    std::string get_request(std::string_view url, std::string_view objects);

    /**
     * @brief
     * @param url Get request to url with callback.
     * @param objects
     * @param callback Called once the response is recieved.
     */
    void get_request_callback(std::string_view url, std::string_view objects,
                              std::function<void(std::string)> callback);

    /**
     * @brief
     * @param url
     * @param objects
     * @param data String to post.
     */
    void post_request(std::string_view url, std::string_view objects, std::string_view data);
}  // namespace ender
