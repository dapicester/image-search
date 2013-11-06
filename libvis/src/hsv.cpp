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
toBgr(const Mat& image, const Vec3i& levels) {
    BOOST_ASSERT(image.depth() == DataType<float>::type);
    BOOST_ASSERT(image.channels() == 3);

    vector<Mat> planes;
    split(image, planes);
    if (sum(levels)[0] > 0) {
        for (int i = 0; i < 3;  i++) planes[i] /= levels[i];
    }
    planes[0] *= 360.;

    Mat bgr;
    merge(planes, bgr);
    cvtColor(bgr, bgr, CV_HSV2BGR);

    return bgr;
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
        channels[i] = imquantize<float>(channels[i], thresh);
        BOOST_ASSERT(channels[i].size() == image.size());
    }

    Mat quantized;
    merge(channels, quantized);
    BOOST_ASSERT(quantized.size() == image.size());

    return quantized;
}

/// Compute the histogram using non-uniform mapping
template <typename T>
Mat
computeHistogram(const Mat& quantized, const Vec3i& levels, bool normalize) {
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

    Mat histogram(values);
    if (normalize) {
        cv::normalize(histogram, histogram, 1, 0, cv::NORM_L1);
    }

    return histogram;
}

HsvExtractor::HsvExtractor(const Vec3i& l, bool f)
        : levels(l), medfilt(f) {}

HsvExtractor::~HsvExtractor() {}

size_t
HsvExtractor::getNumBins() const {
    return levels[0] * levels[1] * levels[2] + levels[2] + 1;
}

Mat
HsvExtractor::extract(const Mat& image, bool normalize, OutputArray& qimage) const {
    Mat hsv = toHsv(image);
    Mat quantized = quantize(hsv, levels + Vec3i(0,1,1));

    if (medfilt) {
        quantized = medfilt2<float>(quantized);
    }

    BOOST_ASSERT(quantized.size() == image.size());

    if (qimage.needed()) {
        // TODO use toBgr() here

        vector<Mat> planes;
        split(quantized, planes);

        Vec3i l = levels + Vec3i(0, 1, 1);
        for (int i = 0; i < 3; i++)
            planes[i] /= l[i];
        planes[0] *= 360.;
        merge(planes, qimage);
        cvtColor(qimage, qimage, CV_HSV2BGR);
    }

    return computeHistogram<float>(quantized, levels, normalize);
}

Scalar colorLevel(int index, Vec3i levels) {
    // TODO move these outside and pass as arguments
    int numColors = levels[0] * levels[1] * levels[2];
    int numGrays = levels[2] + 1;
    Mat grays = linspace(1, 256, numGrays);

    if (index > numColors - 1) { // grays
        int level = index - numColors;
        int value = grays.at<int>(level) - 1;
        return Scalar(value, value, value);
    }
    else { // colors
        Vec3i indices = ind2sub(levels, index); // FIXME ind2sub should be for row-major

        Vec3f temp;
        for (int i = 0; i < 3; i++) {
            temp[i] = static_cast<float>(indices[i] + 1) / levels[i];
            BOOST_ASSERT(temp[i] >= 0. and temp[i] <= 1.);
        }
        temp[0] *= 360.;

        Mat bgr(1, 1, CV_32FC3);
        bgr.at<Vec3f>(0) = temp;

        cvtColor(bgr, bgr, CV_HSV2BGR);

        Vec3f& val = bgr.at<Vec3f>(0);
        val *= 255.;

        return Scalar(val[0], val[1], val[2]);
    }
}

Mat
HsvExtractor::render(const Mat& histogram) const {
    int width = 512, height = 200; // size of the rendered image
    Mat image(height, width, CV_8UC3, Scalar::all(0));

    rectangle(image, Rect(0, 0, width, height), Scalar(210, 210, 210), CV_FILLED);

    int bins = histogram.total();
    int binw = cvRound(static_cast<double>(width)/bins);

    Mat normalized;
    normalize(histogram, normalized, 0, height, NORM_MINMAX);

    MatConstIterator_<float> it = normalized.begin<float>(), end = normalized.end<float>();
    for (int bin = 0; it < end; ++it, ++bin) {
        int binh = cvCeil(*it);

        if (binh == 0) continue;

        Rect rect(binw*bin, height - binh, binw, binh);
        rectangle(image, rect, colorLevel(bin, levels), CV_FILLED);
    }

    return image;
}

} /* namespace vis */

