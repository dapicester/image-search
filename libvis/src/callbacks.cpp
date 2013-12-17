/**
 * @file callbacks.cpp
 * @brief Callbacks for histograms computation.
 * @author Paolo D'Apice
 */

#include "vis/callbacks.hpp"
#include "vis/vocabulary.hpp"

#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

HogBagOfWordsCallback::HogBagOfWordsCallback(const Vocabulary& v)
        : bow(v) {}

arma::fvec
HogBagOfWordsCallback::operator()(const cv::Mat& image) const {
    return bow.extract(image);
}

HsvHistogramsCallback::HsvHistogramsCallback() {}

arma::fvec
HsvHistogramsCallback::operator()(const cv::Mat& image) const {
    return hsv.extract(image);
}

CompositeCallback::CompositeCallback(const Vocabulary& v)
        : hog(v) {}

arma::fvec
CompositeCallback::operator()(const cv::Mat& image) const {
    cv::Mat bwimage;
    cv::cvtColor(image, bwimage, CV_BGR2GRAY);

    arma::fmat d1 = hog(bwimage);
    arma::fvec d2 = hsv(image);

    return arma::join_cols(d1, d2);
}

Callback*
getCallback(DescriptorsType t, const Vocabulary* vocabulary) {
    switch (t) {
    case HOG:
        return new HogBagOfWordsCallback(*vocabulary);
    case HSV:
        return new HsvHistogramsCallback;
    case HOG_HSV:
        return new CompositeCallback(*vocabulary);
    }
}

} /* namespace vis */

