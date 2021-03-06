/**
 * @file standardize.hpp
 * @brief Convert an image to a standard size and format
 * @author Paolo D'Apice
 */

#ifndef VIS_STANDARDIZE_HPP
#define VIS_STANDARDIZE_HPP

#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

namespace standardize {
    static const int HEIGHT = 480; ///< Standard image height
}

/**
 * @brief Convert image into a standard format.
 * @param in Input image
 * @param[out] out A single-precision image with maximum size of @a height.
 * @param height Maximum height
 */
void
standardizeImage(const cv::Mat& in, cv::Mat& out, int height = standardize::HEIGHT);

inline cv::Mat
standardizeImage(const cv::Mat& in, int height = standardize::HEIGHT) {
    cv::Mat out;
    standardizeImage(in, out, height);
    return out;
}

} /* namespace vis */

#endif /* VIS_STANDARDIZE_HPP */

