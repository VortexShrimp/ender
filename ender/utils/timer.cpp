#include "timer.hpp"

namespace ender {
    high_resolution_timer::high_resolution_timer() noexcept {
        QueryPerformanceFrequency(&m_frequency);
        QueryPerformanceCounter(&m_start_time);

        m_last_time = m_start_time;
    }

    float high_resolution_timer::delta_time_seconds() noexcept {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);

        const auto delta_time =
            static_cast<float>(current_time.QuadPart - m_last_time.QuadPart) / m_frequency.QuadPart;

        m_last_time = current_time;

        return delta_time;
    }

    float high_resolution_timer::elapsed_time_seconds() noexcept {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);

        return static_cast<float>(current_time.QuadPart - m_start_time.QuadPart) /
               m_frequency.QuadPart;
    }

    float high_resolution_timer::elapsed_time_milliseconds() noexcept {
        return elapsed_time_seconds() * 1000.f;
    }

}  // namespace ender
