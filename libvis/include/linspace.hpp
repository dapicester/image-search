/**
 * @file linspace.hpp
 * @brief Equivalent of the Matlab linspace function
 * Original code taken from:
 * http://dsj23.wordpress.com/2013/02/13/matlab-linspace-function-written-in-c/
 * @author Paolo D'Apice
 */

#ifndef VIS_LINSPACE_HPP
#define VIS_LINSPACE_HPP

#include <opencv2/core/core.hpp>

namespace vis {

template <typename T>
cv::Mat linspace(T min, T max, int n) {
    cv::Mat result = cv::Mat_<T>(1,n);

    const T range = max - min;
    for (int i = 0; i < n-1; i++) {
        result.at<T>(0,i) = min + i*range/(n - 1);
    }
    result.at<T>(0,n-1) = max;

    return result;
}

} /* namespace vis */

#endif /* VIS_LINSPACE_HPP */

