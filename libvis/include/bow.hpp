/**
 * @file bow.hpp
 * @brief Bag-of-words extractor.
 * @author Paolo D'Apice
 */

#ifndef VIS_BOW_HPP
#define VIS_BOW_HPP

#include "vocabulary.hpp"
#include <armadillo>
#include <boost/noncopyable.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

/// @brief Extracts bag-of-words for the given \c Extractor template.
template <class Extractor>
class BagOfWords : private boost::noncopyable {
public:
    /// Constructor.
    BagOfWords(const Vocabulary& voc) : vocabulary(voc) {}

    /// @brief Compute bag of words of HOG from a gray-scale single-precision image.
    arma::fvec extract(const cv::Mat& image) const;

private:
    const Vocabulary& vocabulary; ///< Reference to the vocabulary instance.
    const Extractor extractor;    ///< Local \c Extractor instance.
};

template <class Extractor>
arma::fvec
BagOfWords<Extractor>::extract(const cv::Mat& image) const {
    arma::fmat descriptors = extractor.extract(image);
    arma::uvec words = vocabulary.lookup(descriptors);
    arma::fvec histogram = arma::conv_to<arma::fvec>::from(arma::hist(words, vocabulary.getNumWords()));
    return histogram / arma::as_scalar(arma::sum(histogram));
}

} /* namespace vis */

#endif /* VIS_BOW_HPP */

