/**
 * @file hsv_aux.cpp
 * @brief Auxiliary functions for HSV color histogram.
 * @author Paolo D'Apice
 */

#include "hsv_aux.hpp"

#include <boost/assert.hpp>

namespace vis {

arma::ivec3
ind2sub(const arma::ivec3& size, int index) {
    arma::ivec3 out;
    arma::ivec3 k{1, size(0), size(0) * size(1)};

    int vi, vj;
    int n = index + 1; // 0-base index
    for (int i = 2; i >= 0; i--) {
        vi = (n - 1) % k[i] + 1;
        vj = (n - vi) / k[i] + 1;

        n = vi;
        out(i) = vj - 1; // 0-base index

        BOOST_ASSERT(n >= 0);
        BOOST_ASSERT(out(i) >= 0);
    }

    return out;
}

cv::Mat
toHsv(const cv::Mat& image) {
    BOOST_ASSERT(image.depth() == cv::DataType<float>::type);

    cv::Mat hsv;
    if (image.channels() == 1) {
        cv::Mat temp;
        cv::cvtColor(image, temp, CV_GRAY2BGR);
        cv::cvtColor(temp,  hsv,  CV_BGR2HSV);
    } else {
        cv::cvtColor(image, hsv, CV_BGR2HSV);
    }

    std::vector<cv::Mat> planes;
    cv::split(hsv, planes);
    planes[0] /= 360.;
    cv::merge(planes, hsv);

    return hsv;
}

cv::Mat
toBgr(const cv::Mat& image, const arma::ivec3& levels) {
    BOOST_ASSERT(image.depth() == cv::DataType<float>::type);
    BOOST_ASSERT(image.channels() == 3);

    std::vector<cv::Mat> planes;
    cv::split(image, planes);
    if (arma::sum(levels) > 0) {
        for (int i = 0; i < 3;  i++) planes[i] /= levels[i];
    }
    planes[0] *= 360.;

    cv::Mat bgr;
    cv::merge(planes, bgr);
    cvtColor(bgr, bgr, CV_HSV2BGR);

    return bgr;
}

cv::Mat
quantize(const cv::Mat& image, const arma::ivec3& levels) {
    BOOST_ASSERT(image.depth() == cv::DataType<float>::type);
    BOOST_ASSERT(image.channels() == 3);

    std::vector<cv::Mat> channels;
    cv::split(image, channels);

    for (int i = 0; i < 3; i++) {
        const float step = 1./levels[i];
        arma::fvec thresh = arma::linspace<arma::fvec>(step, 1.-step, levels[i]-1);
        channels[i] = imquantize(channels[i], thresh);
        BOOST_ASSERT(channels[i].size() == image.size());
    }

    cv::Mat quantized;
    cv::merge(channels, quantized);
    BOOST_ASSERT(quantized.size() == image.size());

    return quantized;
}

cv::Mat medfilt2(const cv::Mat& in) {
    BOOST_ASSERT(in.depth() == cv::DataType<float>::type);
    BOOST_ASSERT(in.isContinuous());

    cv::Mat out(in.size(), in.type());
    cv::medianBlur(in, out, 3); // 3x3 neighborhood

    return out;
}

cv::Mat
imquantize(const cv::Mat& in, const arma::fvec& thresholds) {
    BOOST_ASSERT_MSG(cv::DataType<float>::type == in.type(), "input is not of type float");

    cv::Mat index(in.size(), in.type(), cv::Scalar::all(1));
    for (int i = 0; i < thresholds.size() ; i++) {
        cv::Mat temp = (in > thresholds(i)) / 255;
        temp.convertTo(temp, cv::DataType<float>::type);
        index += temp;
    }

    return index;
}

} /* namespace vis */

