/**
 * @file posixtimer.hpp
 * @brief Simple timer using POSIX functions.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_POSIXTIMER_HPP
#define VIS_UTILS_POSIXTIMER_HPP

#include <sys/time.h>

/// @brief Type used for timestamps.
typedef unsigned long long timestamp_t;

/// @brief Simple timer using POSIX functions.
class [[deprecated]] PosixTimer {
public:
    /// Start measuring time.
    void tic() {
        gettimeofday(&start, NULL);
    }

    /// Get measure of elapsed time since last \c tic().
    void toc() {
        gettimeofday(&stop, NULL);
        elapsed = getMicroseconds(stop) - getMicroseconds(start);
    }

    /// @return Elapsed time in microseconds.
    timestamp_t getMicros() const {
        return elapsed;
    }

    /// @return Elapsed time in milliseconds.
    timestamp_t getMillis() const {
        return elapsed / 1e3;
    }

    /// @return Elapsed time in seconds.
    timestamp_t getSeconds() const {
        return elapsed / 1e6;
    }

private:

    // Get current time in microseconds
    timestamp_t getMicroseconds(const timeval& t) {
        return t.tv_sec * 1000000 + t.tv_usec;
    }

    struct timeval start;
    struct timeval stop;
    timestamp_t elapsed;
};

#endif /* VIS_UTILS_POSIXTIMER_HPP */

