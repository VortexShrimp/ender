#include "internet.hpp"

bool ender::internet_client::create(std::wstring_view user_agent) {
    m_internet = InternetOpen(user_agent.data(), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (m_internet == nullptr) {
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
