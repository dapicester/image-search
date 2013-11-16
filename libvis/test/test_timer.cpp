/**
 * @file test_imer.cpp
 * @brief Usage test for the Timer class.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE timer
#include <boost/test/unit_test.hpp>

#include "vis/utils/timer.hpp"

#include <thread>

#define _CHECK_CLOSE(A,B,DELTA)
#define sleep(X) std::this_thread::sleep_for(std::chrono::seconds((X)))

BOOST_AUTO_TEST_CASE(usage) {
    Timer timer;
    Timer::timestamp_t seconds, millis, micros;

    // tic/toc
    timer.tic();
    sleep(1);
    timer.toc();

    seconds = timer.getSeconds();
    BOOST_CHECK_EQUAL(seconds, 1);

    millis = timer.getMillis();
    BOOST_CHECK_EQUAL(millis, 1e3);

    micros = timer.getMicros();
    BOOST_CHECK_CLOSE(micros, 1e6, .01);

    // again tic/toc
    timer.tic();
    sleep(1);
    timer.toc();

    seconds = timer.getSeconds();
    BOOST_CHECK_EQUAL(seconds, 1);

    millis = timer.getMillis();
    BOOST_CHECK_EQUAL(millis, 1e3);

    micros = timer.getMicros();
    BOOST_CHECK_CLOSE(micros, 1e6, .01);

    // only toc
    sleep(1);
    timer.toc();

    seconds = timer.getSeconds();
    BOOST_CHECK_EQUAL(seconds, 2);

    millis = timer.getMillis();
    BOOST_CHECK_EQUAL(millis, 2e3);

    micros = timer.getMicros();
    BOOST_CHECK_CLOSE(micros, 2e6, .01);
}

