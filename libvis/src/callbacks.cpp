/**
 * @file callbacks.cpp
 * @brief Callbacks for histograms computation.
 * @author Paolo D'Apice
 */

#include "callbacks.hpp"
#include "vocabulary.hpp"
#include "utils/ported.hpp"
#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

HogBagOfWordsCallback::HogBagOfWordsCallback(const Vocabulary* v)
        : bow(v) {}

arma::fvec
HogBagOfWordsCallback::operator()(const cv::Mat& image) const {
    arma::fmat d = hog.extract(image);
    arma::fmat hist = bow(d);
    return hist;
}

HsvHistogramsCallback::HsvHistogramsCallback() {}

arma::fvec
HsvHistogramsCallback::operator()(const cv::Mat& image) const {
    return hsv.extract(image);
}

CompositeCallback::CompositeCallback(const Vocabulary* v)
        : hog(v) {}

arma::fvec
CompositeCallback::operator()(const cv::Mat& image) const {
    cv::Mat bwimage;
    cv::cvtColor(image, bwimage, CV_BGR2GRAY);

    arma::fmat d1 = hog(bwimage);
    arma::fvec d2 = hsv(image);

    return arma::join_cols(d1, d2);
}

} /* namespace vis */

