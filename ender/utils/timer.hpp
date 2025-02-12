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
        /**
         * @brief Set the frequency and start-time of the timer.
         */
        high_resolution_timer() noexcept;

        float delta_time_seconds() noexcept;

        float elapsed_time_seconds() noexcept;
        float elapsed_time_milliseconds() noexcept;

        LARGE_INTEGER frequency() const noexcept;
        LARGE_INTEGER start_time() const noexcept;

    private:
        LARGE_INTEGER m_frequency;
        LARGE_INTEGER m_start_time;
        LARGE_INTEGER m_last_time;
    };

    inline LARGE_INTEGER high_resolution_timer::frequency() const noexcept {
        return m_frequency;
    }

    inline LARGE_INTEGER high_resolution_timer::start_time() const noexcept {
        return m_start_time;
    }
}  // namespace ender
