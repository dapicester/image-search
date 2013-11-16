/**
 * @file utils/conversions.hpp
 * @brief Convert data formats.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_CONVERSIONS_HPP
#define VIS_UTILS_CONVERSIONS_HPP

#include <armadillo>
#include <opencv2/core/core.hpp>

namespace vis {

/// @brief Convert Armadillo matrix to OpenCV matrix.
template <typename T>
cv::Mat
arma2cv(const arma::Mat<T>& from, bool copy = true) {
    cv::Mat mat(from.n_cols, from.n_rows, cv::DataType<T>::type, const_cast<T*>(from.memptr()));
    mat = mat.t();
    return copy ? mat.clone() : mat;
}

/// @brief Convert OpenCV matrix to Armadillo matrix.
template <typename T>
arma::Mat<T>
cv2arma(const cv::Mat& from, bool copy = true) {
    cv::Mat temp(from.t());
    return arma::Mat<T>(temp.ptr<T>(), from.rows, from.cols, copy);
}

} /* namespace vis */

#endif /* VIS_UTILS_CONVERSIONS_HPP */

