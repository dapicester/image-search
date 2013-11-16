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

    /// @brief Returns the number of words in the vocabulary.
    size_t numWords() const { return vocabulary.getNumWords(); }

private:
    const Vocabulary& vocabulary; ///< Reference to the vocabulary instance.
    const Extractor extractor;    ///< Local \c Extractor instance.
};

template <class Extractor>
arma::fvec
BagOfWords<Extractor>::extract(const cv::Mat& image) const {
    using namespace arma;
    fmat descriptors = extractor.extract(image);
    uvec words = vocabulary.lookup(descriptors);
    size_t n = numWords();
    fvec histogram = conv_to<fvec>::from( hist(words, linspace<uvec>(1,n,n)) );
    return histogram / sum(histogram);
}

} /* namespace vis */

#endif /* VIS_BOW_HPP */

