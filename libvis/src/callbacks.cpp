/**
 * @file callbacks.cpp
 * @brief Callbacks for histograms computation.
 * @author Paolo D'Apice
 */

#include "callbacks.hpp"
#include "vocabulary.hpp"
#include "utils/ported.hpp"

namespace vis {

HogVocabularyCallback::HogVocabularyCallback(size_t n)
        : numFeatures(n) {}

cv::Mat
HogVocabularyCallback::operator()(const cv::Mat& image) const {
    cv::Mat d = hog.extract(image).toMat();
    d = colsubset<float>(d, numFeatures, UNIFORM);
    d = d.t(); // TODO avoid transpose
    return d;
}

BagOfWords::BagOfWords(const Vocabulary* v)
        : vocabulary(v) {}

cv::Mat
BagOfWords::operator()(const cv::Mat& descriptors) const {
    cv::Mat words = vocabulary->quantize(descriptors);
    cv::Mat histogram = hist(words, vocabulary->getNumWords(), SUM1);
    return histogram;
}

HogBagOfWordsCallback::HogBagOfWordsCallback(const Vocabulary* v)
        : bow(v) {}

cv::Mat
HogBagOfWordsCallback::operator()(const cv::Mat& image) const {
    cv::Mat d = hog.extract(image).toMat();
    cv::Mat hist = bow(d).t(); // TODO avoid transpose
    return hist;
}

HsvHistogramsCallback::HsvHistogramsCallback() {}

cv::Mat
HsvHistogramsCallback::operator()(const cv::Mat& image) const {
    return hsv.extract(image).t(); // TODO avoid transpose
}

size_t
HsvHistogramsCallback::getNumBins() const {
    return hsv.getNumBins();
}

CompositeCallback::CompositeCallback(const Vocabulary* v)
        : hog(v) {}

cv::Mat
CompositeCallback::operator()(const cv::Mat& image) const {
    cv::Mat d;
    cv::Mat bwimage;
    cv::cvtColor(image, bwimage, CV_BGR2GRAY);
    cv::hconcat(hog(bwimage), hsv(image), d);
    return d;
}

size_t
CompositeCallback::getNumBins() const {
    return hsv.getNumBins();
}

} /* namespace vis */

