#include "timer.hpp"

ender::timer_basic::timer_basic() noexcept {
    QueryPerformanceCounter(&m_start_time);
    QueryPerformanceFrequency(&m_frequency);
}

float ender::timer_basic::time_elapsed_seconds() const noexcept {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);

    return static_cast<float>(current_time.QuadPart - m_start_time.QuadPart) / m_frequency.QuadPart;
}

ender::timer_complex::timer_complex() noexcept : timer_basic() {
    m_last_time = m_start_time;
}

float ender::timer_complex::delta_time_seconds() noexcept {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);

    const auto delta_time =
        static_cast<float>(current_time.QuadPart - m_last_time.QuadPart) / m_frequency.QuadPart;

    m_last_time = current_time;

    return delta_time;
}
