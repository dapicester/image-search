/**
 * @file standardize.cpp
 * @brief Convert an image to a standard size and format
 * @author Paolo D'Apice
 */

#include "vis/standardize.hpp"

namespace vis {

void
standardizeImage(const cv::Mat& in, cv::Mat& out, int height) {
    // single precision
    cv::Mat single;
    in.convertTo(single, CV_32F, 1./255);

    // check size
    if (single.size().height <= height) {
        single.copyTo(out);
        return;
    }

    // resize
    double scale = static_cast<double>(height) / single.size().height;
    cv::resize(single, out, cv::Size(), scale, scale, cv::INTER_LINEAR);
}

} /* namespace vis */

