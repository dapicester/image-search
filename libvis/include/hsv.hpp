/**
 * @file hsv.hpp
 * @brief HSV color histogram
 * @author Paolo D'Apice
 */

#ifndef VIS_HSV_HPP
#define VIS_HSV_HPP

#include <opencv2/core/core.hpp>

namespace vis {

/// Convert a BGR color single-precision image to HSV colorspace.
cv::Mat toHsv(const cv::Mat& image);

/// Quantize image channels to given levels.
cv::Mat quantize(const cv::Mat& image, const cv::Vec3i& levels);

} /* namespace vis */

#endif /* VIS_HSV_HPP */

