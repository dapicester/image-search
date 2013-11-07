/**
 * @file callbacks.hpp
 * @brief Callbacks for histograms computation.
 * @author Paolo D'Apice
 */

#ifndef VIS_CALLBACKS_HPP
#define VIS_CALLBACKS_HPP

#include "descriptors_type.hpp"
#include "hog.hpp"
#include "hsv.hpp"

namespace vis {

class Vocabulary;

/// @brief Base class for callbacks.
template <typename Derived>
struct Callback {
    cv::Mat operator()(const cv::Mat& image) const {
        return static_cast<Derived*>(this)->operator()(image);
    }
};

/// Compute HOGs for constructing a word vocabulary.
struct HogVocabularyCallback : Callback<HogVocabularyCallback> {
    HogVocabularyCallback(size_t numFeatures);
    cv::Mat operator()(const cv::Mat& image) const;
private:
    size_t numFeatures;
    HogExtractor hog;
};

/// Compute bag-of-words using the given vocabulary.
struct BagOfWords : Callback<BagOfWords> {
    BagOfWords(const Vocabulary* vocabulary);
    cv::Mat operator()(const cv::Mat& descriptors) const;
private:
    const Vocabulary* vocabulary;
};

/// Compute HOG bag-of-words.
struct HogBagOfWordsCallback : Callback<BagOfWords> {
    HogBagOfWordsCallback(const Vocabulary* v);
    cv::Mat operator()(const cv::Mat& image) const;

    static const DescriptorsType type = vis::HOG;
private:
    BagOfWords bow;
    HogExtractor hog;
};

/// Compute HSV color histogram.
struct HsvHistogramsCallback : Callback<HsvHistogramsCallback> {
    HsvHistogramsCallback();
    cv::Mat operator()(const cv::Mat& image) const;
    size_t getNumBins() const;

    static const DescriptorsType type = vis::HSV;
private:
    HsvExtractor hsv;
};

/// Compute both HOG bag-of-words and HSV color histogram.
struct CompositeCallback : Callback<CompositeCallback> {
    CompositeCallback(const Vocabulary* v);
    cv::Mat operator()(const cv::Mat& image) const;
    size_t getNumBins() const;

    static const DescriptorsType type = vis::HOG_HSV;
private:
    HogBagOfWordsCallback hog;
    HsvHistogramsCallback hsv;
};

} /* namespace vis */

#endif /* VIS_CALLBACKS_HPP */

