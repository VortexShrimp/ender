/**
 * @file timer.hpp
 * @brief Timer utility functions and API.
 */

#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif  // !WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace ender {
    /**
     * @brief Simple high-resolution timer.
     *
     * The timer starts when the object is created and can be used to measure elapsed time.
     */
    class timer_basic {
    public:
        timer_basic() noexcept;

        /**
         * @brief Returns the elapsed time in seconds since the timer was created.
         * @return Elapsed time in seconds.
         */
        float time_elapsed_seconds() const noexcept;

    protected:
        LARGE_INTEGER m_start_time;
        LARGE_INTEGER m_frequency;
    };

    /**
     * @brief High-resolution timer with delta time calculation.
     *
     * The timer starts when the object is created and can be used to measure elapsed time
     * and delta time between frames.
     */
    class timer_complex : public timer_basic {
    public:
        timer_complex() noexcept;

        float delta_time_seconds() noexcept;

    private:
        LARGE_INTEGER m_last_time;
    };
}  // namespace ender
