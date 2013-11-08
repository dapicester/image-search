/**
 * @file hsv_aux.hpp
 * @brief Auxiliary functions for HSV color histogram.
 * @author Paolo D'Apice
 */

#ifndef VIS_HSV_AUX_HPP
#define VIS_HSV_AUX_HPP

#include <armadillo>
#include <boost/assert.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

/// Equivalent to Matlab @c ind2sub function.
arma::ivec3 ind2sub(const arma::ivec3& size, int index);

/// Convert a BGR color single-precision image to HSV colorspace.
cv::Mat toHsv(const cv::Mat& image);

/// Convert a HSV color single-precision image to BGR colorspace.
cv::Mat toBgr(const cv::Mat& image, const arma::ivec3& levels = arma::ivec3{0,0,0}); // FIXME perché 0,0,0?

/// Quantize image channels to given levels.
cv::Mat quantize(const cv::Mat& image, const arma::ivec3& levels);

/**
 * @brief Equivalent of Matlab medfilt2 function.
 * Input matrix must be transposed.
 */
// TODO refactor
template <typename T>
cv::Mat
medfilt2(const cv::Mat& in) {
    BOOST_ASSERT(in.depth() == cv::DataType<T>::type);
    BOOST_ASSERT(in.isContinuous());

    cv::Mat out(in.size(), in.type());
    cv::medianBlur(in, out, 3); // 3x3 neighborhood

    return out;
}

// FIXME Input matrix must be transposed.
// TODO refactor
template <typename T>
cv::Mat
imquantize(const cv::Mat& in, const cv::Mat& levels) {
    BOOST_ASSERT_MSG(cv::DataType<T>::type == in.type(), "input is not of type T");
    BOOST_ASSERT_MSG(cv::DataType<T>::type == levels.type(), "levels is not of type T");

    size_t N = levels.total();
    cv::Mat index(in.size(), in.type(), cv::Scalar::all(1));

    for (int i = 0; i < N; i++) {
        cv::Mat temp = (in > levels.at<T>(i)) / 255;
        temp = temp.t(); // Matlab store images by columns, OpenCV by rows
        temp.convertTo(temp, cv::DataType<T>::type);
        index += temp.t();
    }

    return index;
}

} /* namespace vis */

#endif /* VIS_HSV_AUX_HPP */

