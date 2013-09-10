/**
 * @file standardize.hpp
 * @brief Rescale an image to a standard size
 * @author Paolo D'Apice
 */

#ifndef VIS_STANDARDIZE_HPP
#define VIS_STANDARDIZE_HPP

#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

void standardizeImage(const cv::Mat& in, cv::Mat& out, int height = 480) {
    // single precision
    cv::Mat temp;
    in.convertTo(temp, CV_32F);

    // resize
    double scale = static_cast<double>(height)/temp.size().height;
    cv::resize(temp, out, cv::Size(), scale, scale);
    // TODO check values are within range with saturate_cast<>
}

} /* namespace vis */

#endif /* VIS_STANDARDIZE_HPP */

