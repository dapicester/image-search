#ifndef VIS_EXTRACT_HPP
#define VIS_EXTRACT_HPP

#include "hog.hpp"
#include "standardize.hpp"
#include "vocabulary.hpp"
#include "utils.hpp"
#include <boost/filesystem/path.hpp>
#include <vector>
#include <cstdio>

namespace vis {

// TODO move to _impl.hpp

/**
 * @brief Extract descriptors on a list of images.
 * @param names List of full pathnames to images
 * @param output Descriptors
 * @param cv Callback computing descriptors
 */
template <typename Callback>
void
extract(const std::vector<boost::filesystem::path>& names, cv::Mat& output, const Callback& cb) {
    cv::Mat descriptors;

    // TODO openmp parfor
    size_t numImages = names.size();
    size_t i = 0;
    std::vector<boost::filesystem::path>::const_iterator it = names.begin(), end = names.end();

    for (; it != end; ++it) {
        const std::string& name = it->string();
        printf("  Extracting features from %s (%lu/%lu)\n", name.c_str(), i+1, numImages);

        cv::Mat input = cv::imread(name, CV_LOAD_IMAGE_GRAYSCALE); // FIXME only hog needs grayscale
        cv::Mat image; // TODO change standardize signature to allow return?
        standardizeImage(input, image);

        cv::Mat m = cb(image);

        descriptors.push_back(m);
        ++i;
    }
    output = descriptors.t();
}

/// Compute HOGs for constructing a word vocabulary.
struct HogVocabularyCallback {
    HogVocabularyCallback(size_t n) : numFeatures(n) {}

    cv::Mat operator()(const cv::Mat& image) const {
        cv::Mat d = hog.extract(image).toMat();
        d = colsubset<float>(d, numFeatures, UNIFORM);
        d = d.t();
        return d;
    }
private:
    size_t numFeatures;
    HogExtractor hog;
};

/// Compute bag-of-words using the given vocabulary.
struct BagOfWords {
    BagOfWords(const Vocabulary* v) : vocabulary(v), numWords(v->getNumWords()) {}

    cv::Mat operator()(const cv::Mat& descriptors) const {
        cv::Mat words = vocabulary->quantize(descriptors);
        cv::Mat histogram = hist(words, numWords, SUM1);
        return histogram;
    }
private:
    const Vocabulary* vocabulary;
    size_t numWords;
};

/// Compute HOG bag-of-words.
struct HogBagOfWordsCallback {
    HogBagOfWordsCallback(const Vocabulary* v) : bow(v) {}

    cv::Mat operator()(const cv::Mat& image) const {
        cv::Mat d = hog.extract(image).toMat();
        cv::Mat hist = bow(d).t();
        return hist;
    }
private:
    BagOfWords bow;
    HogExtractor hog;
};

} /* namespace vis */

#endif /* VIS_EXTRACT_HPP */

