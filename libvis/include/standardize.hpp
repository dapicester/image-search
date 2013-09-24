/**
 * @file standardize.hpp
 * @brief Rescale an image to a standard size
 * @author Paolo D'Apice
 */

#ifndef VIS_STANDARDIZE_HPP
#define VIS_STANDARDIZE_HPP

#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

/**
 * @brief Convert image into a standard format.
 * @param in Input image
 * @param[out] out A single-precision image with maximum size of @a height.
 * @param height Maximum height
 */
void
standardizeImage(const cv::Mat& in, cv::Mat& out, int height = 480) {
    // single precision
    cv::Mat single;
    in.convertTo(single, CV_32F);

    // check size
    if (single.size().height <= height) {
        single.copyTo(out);
        return;
    }

    // resize
    double scale = static_cast<double>(height)/single.size().height;
    cv::resize(single, out, cv::Size(), scale, scale);
}

} /* namespace vis */

#endif /* VIS_STANDARDIZE_HPP */

