/**
 * @file armadillo_usage.cpp
 * @brief Usage test for Armadillolibrary
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE armadillo
#include <boost/test/unit_test.hpp>

#include <armadillo>

using namespace arma;

BOOST_AUTO_TEST_CASE(arma_mat) {
    mat a = randu<mat>(4,4);
    a.print("a:");
}

