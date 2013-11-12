/**
 * @file callbacks_hog.hpp
 * @brief Callbacks for bag-of-words computation.
 * @author Paolo D'Apice
 */

#ifndef VIS_CALLBACKS_HOG_HPP
#define VIS_CALLBACKS_HOG_HPP

#include "hog.hpp"

namespace vis {

class Vocabulary;

/// Compute HOGs for constructing a word vocabulary.
struct HogVocabularyCallback {
    HogVocabularyCallback(size_t numFeatures);
    arma::fmat operator()(const cv::Mat& image) const;
private:
    size_t numFeatures;
    HogExtractor hog;
};

/// Compute bag-of-words using the given vocabulary.
struct BagOfWords {
    BagOfWords(const Vocabulary* vocabulary);
    arma::fmat operator()(const arma::fmat& descriptors) const;
    size_t numWords() const;
private:
    const Vocabulary* vocabulary;
};

} /* namespace vis */

#endif /* VIS_CALLBACKS_HOG_HPP */

