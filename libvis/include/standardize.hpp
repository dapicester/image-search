/**
 * @file standardize.hpp
 * @brief Rescale an image to a standard size
 * @author Paolo D'Apice
 */

#ifndef VIS_STANDARDIZE_HPP
#define VIS_STANDARDIZE_HPP

#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

static const double HEIGHT = 480.0;

void standardizeImage(const cv::Mat& in, cv::Mat& out, double height = HEIGHT) {
    double scale = height/in.size().height;
    cv::resize(in, out, cv::Size(), scale, scale);
    // TODO check values are within range with saturate_cast<>
}

} /* namespace vis */

#endif /* VIS_STANDARDIZE_HPP */

