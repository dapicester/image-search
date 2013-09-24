/**
 * @file standardize.hpp
 * @brief Convert an image to a standard size and format
 * @author Paolo D'Apice
 */

#ifndef VIS_STANDARDIZE_HPP
#define VIS_STANDARDIZE_HPP

#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

static const unsigned short IMG_HEIGHT = 480; ///< Standard image height
static const float IMG_SCALE = 1./255;        ///< Scaling factor
static const float IMG_DELTA = 0.001546;      ///< Delta added to scaled values

/**
 * @brief Convert image into a standard format.
 * @param in Input image
 * @param[out] out A single-precision image with maximum size of @a height.
 * @param height Maximum height
 */
void
standardizeImage(const cv::Mat& in, cv::Mat& out, int height = IMG_HEIGHT) {
    // single precision
    cv::Mat single;
    in.convertTo(single, CV_32F, IMG_SCALE, IMG_DELTA);

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

