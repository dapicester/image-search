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

/// Equivalent of Matlab \c medfilt2 function.
cv::Mat medfilt2(const cv::Mat& in);

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

/**
 * @brief Equivalent of the Matlab linspace function
 * Original code taken from:
 * http://dsj23.wordpress.com/2013/02/13/matlab-linspace-function-written-in-c/
 */
// TODO move to hsv_aux
template <typename T>
cv::Mat
linspace(T min, T max, size_t n) {
    cv::Mat result = cv::Mat_<T>(1,n);
    T* ptr = result.ptr<T>(0);
    const T range = max - min;
    for (size_t i = 0; i < n-1; i++) {
        ptr[i] = min + i*range/(n - 1);
    }
    ptr[n-1] = max;

    return result;
}

/**
 * @brief Equivalent of Matlab reshape function.
 */
inline
cv::Mat
reshape(const cv::Mat& in, size_t cols) {
    // TODO avoid transpose
    cv::Mat out = in.t();
    return out.reshape(0, cols).t();
}



} /* namespace vis */

#endif /* VIS_HSV_AUX_HPP */

