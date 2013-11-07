/**
 * @file utils/matrix.hpp
 * @brief Utility methods for OpenCV matrices
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_MATRIX_HPP
#define VIS_TEST_UTILS_MATRIX_HPP

#include <armadillo>
#include <opencv2/core/core.hpp>

namespace test {

/// @return true if matrices are equals
static
bool
equals(const cv::Mat& a, const cv::Mat& b) {
    if (a.size() != b.size()) return false;
    cv::Mat diff = (a != b);
    return cv::countNonZero(diff) == 0 ? true : false;
}

/// @return true if matrices are equals
template <typename T>
bool
equals(const arma::Mat<T>& a, const arma::Mat<T>& b) {
    if (a.n_rows != b.n_rows or a.n_cols != b.n_cols) return false;
    arma::umat diff = (a == b);
    return all(vectorise(diff));
}

/// @return true if matrix minimum and maximum matches the given values
static
bool
hasMinMax(const cv::Mat& in, double min, double max) {
    double min_, max_;
    cv::minMaxIdx(in, &min_, &max_);
    return ( (max >= max_) and (min <= min_));
}

} /* namespace test */

#endif /* VIS_TEST_UTILS_MATRIX_HPP */

