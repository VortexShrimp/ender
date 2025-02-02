#include "timer.hpp"

ender::high_resolution_timer::high_resolution_timer() {
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_last_time);
}

float ender::high_resolution_timer::get_delta_time() {
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);

    const auto delta_time =
        static_cast<float>(current_time.QuadPart - m_last_time.QuadPart) / m_frequency.QuadPart;

    m_last_time = current_time;

    return delta_time;
}
