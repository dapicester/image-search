/**
 * @file armadillo_usage.cpp
 * @brief Usage test for Armadillolibrary
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE armadillo
#include <boost/test/unit_test.hpp>

#include "utils/matrix.hpp"

#include <armadillo>

using namespace arma;

BOOST_AUTO_TEST_CASE(arma_mat) {
    mat a = randu<mat>(4,4);
    a.print("randu");
}

BOOST_AUTO_TEST_CASE(arma_vec) {
    vec c{1,2,3};
    rowvec r{1,2,3};

    c.print("colvec");
    r.print("rowvec");
}

BOOST_AUTO_TEST_CASE(arma_cube) {
    int data[] = {1,2,3,4,5,6,7,8,9,10,11,12};
    icube c(data, 3, 2, 2);
    c.print("cube:");

    c.reshape(1, 3*2*2, 1);
    c.print("reshaped:");

    irowvec v = c;
    v.print("vector:");

    imat m = v;
    m.print("matrix:");

    join_cols(m,v).print("join cols");
    join_rows(m,v).print("join rows");
}

BOOST_AUTO_TEST_CASE(arma_reshape) {
    mat a = { 1,2,3,4,5,6 };
    a.reshape(2,3);
    a.print("2x3:");

    a.reshape(3,2);
    a.print("3x2:");

    cube b(a.memptr(), 1, 2, 3);
    b.print("1x2x3:");

    b.reshape(3,2,1);
    b.print("3,2,1:");

    mat c = b;
    c.print("cube to mat:");
}

BOOST_AUTO_TEST_CASE(arma_hist) {
    ivec a = conv_to<ivec>::from(randu<vec>(1000) * 100.);

    ivec b = linspace<ivec>(1,10,10);
    b.print("bins");

    uvec h = hist(a, b);
    h.print("hist");
}

