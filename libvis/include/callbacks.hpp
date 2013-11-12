/**
 * @file callbacks.hpp
 * @brief Callbacks for histograms computation.
 * @author Paolo D'Apice
 */

#ifndef VIS_CALLBACKS_HPP
#define VIS_CALLBACKS_HPP

#include "descriptors_type.hpp"
#include "bow.hpp"
#include "hog.hpp"
#include "hsv.hpp"

namespace vis {

class Vocabulary;

/// @brief Base class for callbacks.
template <typename Derived, DescriptorsType t>
struct Callback {
    /// DescriptorsType of the callback.
    static const DescriptorsType type = t;

    arma::fvec operator()(const cv::Mat& image) const {
        return static_cast<Derived*>(this)->operator()(image);
    }

    size_t length() const {
        return static_cast<Derived*>(this)->length();
    }
};

/// Compute HOG bag-of-words.
struct HogBagOfWordsCallback : Callback<HogBagOfWordsCallback, vis::HOG> {
    HogBagOfWordsCallback(const Vocabulary* v);

    arma::fvec operator()(const cv::Mat& image) const;

    size_t length() const { return bow.numWords(); }

private:
    BagOfWords<HogExtractor> bow;
};

/// Compute HSV color histogram.
struct HsvHistogramsCallback : Callback<HsvHistogramsCallback, vis::HSV> {
    HsvHistogramsCallback();

    arma::fvec operator()(const cv::Mat& image) const;

    size_t length() const { return hsv.getNumBins(); }

private:
    HsvExtractor hsv;
};

/// Compute both HOG bag-of-words and HSV color histogram.
struct CompositeCallback : Callback<CompositeCallback, vis::HOG_HSV> {
    CompositeCallback(const Vocabulary* v);

    arma::fvec operator()(const cv::Mat& image) const;

    size_t length() const { return hsv.length() + hog.length(); }

private:
    HogBagOfWordsCallback hog;
    HsvHistogramsCallback hsv;
};

} /* namespace vis */

#endif /* VIS_CALLBACKS_HPP */

