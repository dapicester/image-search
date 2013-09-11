/**
 * @file test_utils.hpp
 * @brief Test utilities
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_HPP
#define VIS_TEST_UTILS_HPP

#include <opencv2/core/core.hpp>
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

#endif /* VIS_TEST_UTILS_HPP */

