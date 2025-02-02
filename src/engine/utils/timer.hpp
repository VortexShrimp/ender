#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ender {
    /**
     * @brief Uses QueryPerformanceCounter to keep time.
     */
    class high_resolution_timer {
    public:
        high_resolution_timer();

        float get_delta_time();

    private:
        LARGE_INTEGER m_frequency;
        LARGE_INTEGER m_last_time;
    };
}  // namespace ender
