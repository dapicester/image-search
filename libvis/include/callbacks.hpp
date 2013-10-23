/**
 * @file callbacks.hpp
 * @brief Callbacks for histograms computation.
 * @author Paolo D'Apice
 */

#ifndef VIS_CALLBACKS_HPP
#define VIS_CALLBACKS_HPP

#include "hog.hpp"
#include "hsv.hpp"

namespace vis {

/// Compute HOGs for constructing a word vocabulary.
struct HogVocabularyCallback {
    HogVocabularyCallback(size_t numFeatures);
    cv::Mat operator()(const cv::Mat& image) const;
private:
    size_t numFeatures;
    HogExtractor hog;
};

class Vocabulary;

/// Compute bag-of-words using the given vocabulary.
struct BagOfWords {
    BagOfWords(const Vocabulary* vocabulary);
    cv::Mat operator()(const cv::Mat& descriptors) const;
private:
    const Vocabulary* vocabulary;
};

/// Compute HOG bag-of-words.
struct HogBagOfWordsCallback {
    HogBagOfWordsCallback(const Vocabulary* v);
    cv::Mat operator()(const cv::Mat& image) const;
private:
    BagOfWords bow;
    HogExtractor hog;
};

/// Compute HSV color histogram.
struct HsvHistogramsCallback {
    HsvHistogramsCallback();
    cv::Mat operator()(const cv::Mat& image) const;
    size_t getNumBins() const;
private:
    HsvExtractor hsv;
};

struct CompositeCallback {
    CompositeCallback(const Vocabulary* v);
    cv::Mat operator()(const cv::Mat& image) const;
    size_t getNumBins() const;
private:
    HogBagOfWordsCallback hog;
    HsvHistogramsCallback hsv;
};

} /* namespace vis */

#endif /* VIS_CALLBACKS_HPP */

