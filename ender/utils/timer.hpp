#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ender {
    class timer_basic {
    public:
        timer_basic() noexcept;

        float time_elapsed_seconds() const noexcept;

    protected:
        LARGE_INTEGER m_start_time;
        LARGE_INTEGER m_frequency;
    };

    class timer_complex : public timer_basic {
    public:
        timer_complex() noexcept;

        float delta_time_seconds() noexcept;

    private:
        LARGE_INTEGER m_last_time;
    };
}  // namespace ender
