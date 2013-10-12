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

void filter(Mat& image) {
    // TODO median filter
}

/// Compute the histogram using non-uniform mapping
template <typename T>
Mat
computeHistogram(const Mat& quantized, const Vec3i& levels) {
    BOOST_ASSERT(quantized.depth() == DataType<T>::type);

    const Size& sz = quantized.size();
    Mat data = quantized.reshape(1, sz.width * sz.height);

    BOOST_ASSERT(Size(3, sz.width * sz.height) == data.size());
    BOOST_ASSERT(1 == data.channels());

    const int& hue_bins = levels[0];
    const int& sat_bins = levels[1];
    const int& val_bins = levels[2];

    vector< vector< vector<int> > > colors;
    colors.resize(hue_bins);
    for (int i = 0; i < hue_bins; ++i) {
        colors[i].resize(sat_bins);
        for (int j = 0; j < sat_bins; ++j) {
            colors[i][j].resize(val_bins);
        }
    }
    vector<int> grays(val_bins + 1);

    // for each HSV tuple increment the corresponding histogram bin
    for (int i = 0; i < data.rows; i++) {
        Mat t = data.row(i);

        const T& h = t.at<T>(0);
        const T& s = t.at<T>(1);
        const T& v = t.at<T>(2);

        if (v == 1) { // black
            grays[0]++;
        }
        else if (s == 1 and v > 1) { // grays
            grays[v - 1]++;
        }
        else {
            colors[h - 1][s - 2][v - 2]++;
        }
    }

    vector<T> values;
    for (int i = 0; i < hue_bins; ++i) {
        for (int j = 0; j < sat_bins; ++j) {
            const vector<int>& v = colors[i][j];
            values.insert(values.end(), v.begin(), v.end());
        }
    }
    values.insert(values.end(), grays.begin(), grays.end());

    Mat histogram;
    cv::normalize(Mat(values), histogram, 1, 0, cv::NORM_L1);

    return histogram;
}

HsvExtractor::HsvExtractor(const Vec3i& l, bool f)
        : levels(l), medfilt(f) {}

HsvExtractor::~HsvExtractor() {}

Mat
HsvExtractor::extract(const Mat& image) const {
    Mat hsv = toHsv(image);
    Mat quantized = quantize(hsv, levels + Vec3i(0,1,1));

    if (medfilt) {
        filter(quantized);
    }

    BOOST_ASSERT(quantized.size() == image.size());

    return computeHistogram<float>(quantized, levels);
}

} /* namespace vis */

