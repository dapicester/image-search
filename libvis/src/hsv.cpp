/**
 * @file hsv.cpp
 * @brief HSV color histogram
 * @author Paolo D'Apice
 */

#include "hsv.hpp"
#include "hsv_aux.hpp"
#include "utils/conversions.hpp"
#include <boost/assert.hpp>

namespace vis {

/// Compute the histogram using non-uniform mapping
template <typename T>
arma::fvec
computeHistogram(const cv::Mat& quantized, const arma::ivec3& levels, bool normalize) {
    BOOST_ASSERT(quantized.depth() == cv::DataType<T>::type);

    const cv::Size& sz = quantized.size();
    cv::Mat data = quantized.reshape(1, sz.width * sz.height);

    BOOST_ASSERT(cv::Size(3, sz.width * sz.height) == data.size());
    BOOST_ASSERT(1 == data.channels());

    const int& hue_bins = levels[0];
    const int& sat_bins = levels[1];
    const int& val_bins = levels[2];

    arma::icube colors(hue_bins, sat_bins, val_bins);
    colors.zeros();

    arma::ivec grays(val_bins + 1);
    grays.zeros();

    // for each HSV tuple increment the corresponding histogram bin
    for (int i = 0; i < data.rows; i++) {
        cv::Mat t = data.row(i);

        const T& h = t.at<T>(0);
        const T& s = t.at<T>(1);
        const T& v = t.at<T>(2);

        if (v == 1) { // black
            grays(0)++;
        }
        else if (s == 1 and v > 1) { // grays
            grays(v-1)++;
        }
        else {
            colors(h-1, s-2, v-2)++;
        }
    }

    colors.reshape(hue_bins * sat_bins * val_bins, 1, 1);
    arma::ivec values = arma::join_cols(arma::ivec(colors), grays);

    return arma::conv_to<arma::fvec>::from(values) /= arma::as_scalar(arma::sum(values));
}

HsvExtractor::HsvExtractor(const hsv::Levels& l, bool f)
        : levels(l), hsvlevels(l + hsv::Levels{0,1,1}), medfilt(f) {}

HsvExtractor::~HsvExtractor() {}

size_t
HsvExtractor::getNumBins() const {
    return levels[0] * levels[1] * levels[2] + levels[2] + 1;
}

arma::fvec
HsvExtractor::extract(const cv::Mat& image, bool normalize, cv::OutputArray& qimage) const {
    cv::Mat hsv = toHsv(image);
    cv::Mat quantized = quantize(hsv, hsvlevels);

    if (medfilt) {
        quantized = medfilt2(quantized);
    }

    BOOST_ASSERT(quantized.size() == image.size());

    if (qimage.needed()) {
        cv::Mat render = toBgr(quantized, hsvlevels);
        render.copyTo(qimage);
    }

    return computeHistogram<float>(quantized, levels, normalize);
}

cv::Scalar colorLevel(int index, hsv::Levels levels) {
    // TODO move these outside and pass as arguments?
    int numColors = levels[0] * levels[1] * levels[2];
    int numGrays = levels[2] + 1;
    arma::ivec grays = arma::linspace<arma::ivec>(1, 256, numGrays);

    if (index > numColors - 1) { // grays
        int level = index - numColors;
        int value = grays(level) - 1;
        return cv::Scalar(value, value, value);
    }
    else { // colors
        hsv::Levels indices = ind2sub(levels, index);

        cv::Vec3f temp;
        for (int i = 0; i < 3; i++) {
            temp[i] = static_cast<float>(indices[i] + 1) / levels[i];
            BOOST_ASSERT(temp[i] >= 0. and temp[i] <= 1.);
        }
        temp[0] *= 360.;

        cv::Mat bgr(1, 1, CV_32FC3);
        bgr.at<cv::Vec3f>(0) = temp;

        cv::cvtColor(bgr, bgr, CV_HSV2BGR);

        cv::Vec3f& val = bgr.at<cv::Vec3f>(0);
        val *= 255.;

        return cv::Scalar(val[0], val[1], val[2]);
    }
}

cv::Mat
HsvExtractor::render(const arma::fmat& histogram) const {
    int width = 512, height = 200; // size of the rendered image
    cv::Mat image(height, width, CV_8UC3, cv::Scalar::all(0));

    // background color
    cv::rectangle(image, cv::Rect(0, 0, width, height), cv::Scalar(210, 210, 210), CV_FILLED);

    int bins = histogram.size();
    int binw = cvRound(static_cast<double>(width)/bins);

    cv::Mat normalized;
    normalize(arma2cv(histogram), normalized, 0, height, cv::NORM_MINMAX);

    cv::MatConstIterator_<float> it = normalized.begin<float>(), end = normalized.end<float>();
    for (int bin = 0; it < end; ++it, ++bin) {
        int binh = cvCeil(*it);

        if (binh == 0) continue;

        cv::Rect rect(binw*bin, height - binh, binw, binh);
        cv::rectangle(image, rect, colorLevel(bin, levels), CV_FILLED);
    }

    return image;
}

} /* namespace vis */

