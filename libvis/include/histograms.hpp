/**
 * @file histograms.hpp
 * @brief Functions computing histograms
 * @author Paolo D'Apice
 */

#ifndef VIS_HISTOGRAMS_HPP
#define VIS_HISTOGRAMS_HPP

#include "vocabulary.hpp"
#include "utils/ported.hpp"
#include <string>
#include <opencv2/core/core.hpp>

namespace vis {

cv::Mat
bowHistogram(const cv::Mat& descriptors, const Vocabulary& vocabulary) {
    cv::Mat words = vocabulary.quantize(descriptors);
    size_t numWords = vocabulary.getNumWords();
    cv::Mat histogram = hist(words, numWords, SUM1);
    return histogram;
}

} /* namespace vis */

#endif /* VIS_HISTOGRAMS_HPP */

