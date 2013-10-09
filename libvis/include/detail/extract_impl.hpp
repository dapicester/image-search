/**
 * @file detail/extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_EXTRACT_IMPL_HPP
#define VIS_DETAIL_EXTRACT_IMPL_HPP

#include "standardize.hpp"
#include "utils/ported.hpp"
#include "vocabulary.hpp"
#include <cstdio>

namespace vis {

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

HogVocabularyCallback::HogVocabularyCallback(size_t n)
        : numFeatures(n) {}

cv::Mat
HogVocabularyCallback::operator()(const cv::Mat& image) const {
    cv::Mat d = hog.extract(image).toMat();
    d = colsubset<float>(d, numFeatures, UNIFORM);
    d = d.t();
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
    cv::Mat hist = bow(d).t();
    return hist;
}

} /* namespace vis */

#endif /* VIS_DETAIL_EXTRACT_IMPL_HPP */

