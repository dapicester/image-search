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

HogVocabularyCallback::HogVocabularyCallback(size_t n)
        : numFeatures(n) {}

arma::fmat
HogVocabularyCallback::operator()(const cv::Mat& image) const {
    arma::fmat d = hog.extract(image);
    return colsubset(d, numFeatures);
}

BagOfWords::BagOfWords(const Vocabulary* v)
        : vocabulary(v) {}

arma::fmat
BagOfWords::operator()(const arma::fmat& descriptors) const {
    arma::fmat words = vocabulary->quantize(descriptors);
    arma::fmat histogram = arma::conv_to<arma::fmat>::from(arma::hist(words, vocabulary->getNumWords()));
    return histogram / arma::as_scalar(arma::sum(histogram));
}

HogBagOfWordsCallback::HogBagOfWordsCallback(const Vocabulary* v)
        : bow(v) {}

arma::fmat
HogBagOfWordsCallback::operator()(const cv::Mat& image) const {
    arma::fmat d = hog.extract(image);
    arma::fmat hist = bow(d);
    return hist;
}

HsvHistogramsCallback::HsvHistogramsCallback() {}

arma::fmat
HsvHistogramsCallback::operator()(const cv::Mat& image) const {
    return hsv.extract(image);
}

size_t
HsvHistogramsCallback::getNumBins() const {
    return hsv.getNumBins();
}

CompositeCallback::CompositeCallback(const Vocabulary* v)
        : hog(v) {}

arma::fmat
CompositeCallback::operator()(const cv::Mat& image) const {
    cv::Mat bwimage;
    cv::cvtColor(image, bwimage, CV_BGR2GRAY);

    arma::fmat d1 = hog(bwimage);
    arma::fmat d2 = hsv(image);

    // FIXME
    return arma::join_cols(d1, d2);
}

size_t
CompositeCallback::getNumBins() const {
    return hsv.getNumBins();
}

} /* namespace vis */

