/**
 * @file hsv.cpp
 * @brief HSV color histogram
 * @author Paolo D'Apice
 */

#include "hsv.hpp"
#include "utils/ported.hpp"
#include <boost/assert.hpp>

namespace vis {

using namespace cv;

Mat
toHsv(const Mat& image) {
    BOOST_ASSERT(image.depth() == DataType<float>::type);

    Mat hsv;
    if (image.channels() == 1) {
        Mat temp;
        cvtColor(image, temp, CV_GRAY2BGR);
        cvtColor(temp,  hsv,  CV_BGR2HSV);
    } else {
        cvtColor(image, hsv, CV_BGR2HSV);
    }

    vector<Mat> planes;
    split(hsv, planes);
    planes[0] /= 360.;
    merge(planes, hsv);

    return hsv;
}

Mat
quantize(const Mat& image, const Vec3i& levels) {
    BOOST_ASSERT(image.depth() == DataType<float>::type);
    BOOST_ASSERT(image.channels() == 3);

    vector<Mat> channels;
    split(image, channels);

    for (int i = 0; i < 3; i++) {
        float step = 1./levels[i];
        Mat thresh = linspace<float>(step, 1. - step, levels[i] - 1);
        channels[i] = imquantize<float>(channels[i].t(), thresh);
    }

    Mat quantized;
    merge(channels, quantized);

    return quantized;
}

} /* namespace vis */

