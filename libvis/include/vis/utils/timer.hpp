/**
 * @file timer.hpp
 * @brief Simple timer using C++11 functions.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_TIMER_HPP
#define VIS_UTILS_TIMER_HPP

#include <chrono>

/// @brief Simple timer using POSIX functions.
class Timer {

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point stop;

    template <typename time_unit> time_unit elapsed() const {
        return std::chrono::duration_cast<time_unit>(stop - start);
    }

public:

    /// @brief Type used for timestamps.
    typedef unsigned long long timestamp_t;

    /// Start measuring time.
    void tic() {
        start = std::chrono::steady_clock::now();
    }

    /// Get measure of elapsed time since last \c tic().
    void toc() {
        stop = std::chrono::steady_clock::now();
    }

    /// @return Elapsed time in microseconds.
    timestamp_t getMicros() const {
        return elapsed<std::chrono::microseconds>().count();
    }

    /// @return Elapsed time in milliseconds.
    timestamp_t getMillis() const {
        return elapsed<std::chrono::milliseconds>().count();
    }

    /// @return Elapsed time in seconds.
    timestamp_t getSeconds() const {
        return elapsed<std::chrono::seconds>().count();
    }

};

#endif /* VIS_UTILS_TIMER_HPP */

