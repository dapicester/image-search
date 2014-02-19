/**
 * @file test_conversions.cpp
 * @brief Test for Armadillo/OpenCV data conversion.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE conversions
#include <boost/test/unit_test.hpp>

#include "utils/matrix.hpp"
#include "utils/print.hpp"

#include "vis/utils/conversions.hpp"

BOOST_AUTO_TEST_CASE(test_arma2cv) {
    arma::Mat<int> input = { 1, 2, 3, 4, 5, 6 };
    input.reshape(2, 3); // column-major: [1 3 5;
                         //                2 4 5]
    cv::Mat expected = (cv::Mat_<int>(2,3) << 1,3,5,2,4,6); // row-major
    cv::Mat actual = vis::arma2cv(input);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(actual, expected));
}

BOOST_AUTO_TEST_CASE(test_cv2arma) {
    cv::Mat input = (cv::Mat_<int>(2,3) << 1,2,3,4,5,6); // row-major: [1 2 3;
                                                         //             4 5 6]
    arma::Mat<int> expected;
    expected << 1 << 2 << 3 << arma::endr
             << 4 << 5 << 6 << arma::endr; // column-major

    arma::Mat<int> actual = vis::cv2arma<int>(input);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(actual, expected));
}

