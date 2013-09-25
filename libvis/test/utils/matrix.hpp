/**
 * @file utils/matrix.hpp
 * @brief Utility methods for OpenCV matrices
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_MATRIX_HPP
#define VIS_TEST_UTILS_MATRIX_HPP

#include <opencv2/core/core.hpp>

namespace vis {

/// @return true if matrices are equals
bool
equals(const cv::Mat& a, const cv::Mat& b) {
    if (a.size() != b.size()) return false;
    cv::Mat diff = (a != b);
    return cv::countNonZero(diff) == 0 ? true : false;
}

} /* namespace vis */

#endif /* VIS_TEST_UTILS_MATRIX_HPP */

