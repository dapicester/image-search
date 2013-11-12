/**
 * @file callbacks_hog.hpp
 * @brief Callbacks for bag-of-words computation.
 * @author Paolo D'Apice
 */

#ifndef VIS_CALLBACKS_HOG_HPP
#define VIS_CALLBACKS_HOG_HPP

#include <armadillo>

namespace vis {

class Vocabulary;

namespace hog {

/// Compute bag-of-words using the given vocabulary.
// FIXME usare classe BagOfWords
struct BagOfWords {
    BagOfWords(const Vocabulary* vocabulary);
    arma::fmat operator()(const arma::fmat& descriptors) const;
    size_t numWords() const;
private:
    const Vocabulary* vocabulary;
};

} /* namespace hog */
} /* namespace vis */

#endif /* VIS_CALLBACKS_HOG_HPP */

