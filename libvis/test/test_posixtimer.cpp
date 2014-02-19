/**
 * @file test_posixtimer.cpp
 * @brief Usage test for the PosixTimer class.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE posixtimer
#include <boost/test/unit_test.hpp>

#include "vis/utils/posixtimer.hpp"

#define _CHECK_CLOSE(A,B,DELTA)

BOOST_AUTO_TEST_CASE(usage) {
    PosixTimer timer;
    timestamp_t seconds, millis, micros;

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

