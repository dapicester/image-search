/**
 * @file callbacks_hog.cpp
 * @brief Callbacks for bag-of-words computation.
 * @author Paolo D'Apice
 */

#include "callbacks_hog.hpp"
#include "vocabulary.hpp"
#include "utils/conversions.hpp"
#include "utils/ported.hpp"

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
    // FIXME rewrite bow callback implementation
    //arma::fmat histogram = arma::conv_to<arma::fmat>::from(arma::hist(words, vocabulary->getNumWords()));
    //return histogram / arma::as_scalar(arma::sum(histogram));
    cv::Mat temp = arma2cv(words);
    cv::Mat histogram = hist(temp, vocabulary->getNumWords(), SUM1);
    return cv2arma<float>(histogram);
}

} /* namespace vis */

