/**
 * @file test_utils.hpp
 * @brief Test utilities
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_HPP
#define VIS_TEST_UTILS_HPP

#include <opencv2/core/core.hpp>
extern "C" {
#include <vl/random.h>
}
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

/// @return true if equals
bool equals(const cv::Mat& a, const cv::Mat& b) {
    if (a.size() != b.size()) return false;
    cv::Mat diff = (a != b);
    return cv::countNonZero(diff) == 0 ? true : false;
}

template <typename T>
cv::Mat getTestData(int dimension, int numData) {
    VlRand rand;
    vl_rand_init(&rand);
    vl_rand_seed(&rand, 1000);

    cv::Mat data = cv::Mat_<T>(dimension, numData);
    for(int i = 0; i < dimension; i++) {
        for(int j = 0; j < numData; j++) {
            T randomNum = (T) vl_rand_real3(&rand) + 1;
            data.at<T>(i,j) = randomNum;
        }
    }
    return data;
}

#endif /* VIS_TEST_UTILS_HPP */

