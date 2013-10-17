/**
 * @file extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_EXTRACT_HPP
#define VIS_EXTRACT_HPP

#include "hog.hpp"
#include "hsv.hpp"
#include <boost/filesystem/path.hpp>
#include <vector>

namespace vis {

enum LoadImage {
    COLORS,
    GRAYSCALE
};

/**
 * @brief Extract descriptors on a list of images.
 * @param names List of full pathnames to images
 * @param output Descriptors
 * @param cv Callback computing descriptors
 */
template <typename Callback>
void
extract(const std::vector<boost::filesystem::path>& names,
        cv::Mat& output,
        const Callback& cb,
        LoadImage flag = COLORS);

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

#include "detail/extract_impl.hpp"

#endif /* VIS_EXTRACT_HPP */

