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

/// @brief Interface for callbacks.
struct Callback {
    virtual arma::fvec operator()(const cv::Mat& image) const = 0;
    virtual DescriptorsType type() const = 0;
    virtual size_t length() const = 0;
};

/// Compute HOG bag-of-words.
struct HogBagOfWordsCallback : Callback {
    HogBagOfWordsCallback(const Vocabulary& v);

    arma::fvec operator()(const cv::Mat& image) const;

    DescriptorsType type() const { return vis::HOG; }
    size_t length() const { return bow.numWords(); }

private:
    BagOfWords<HogExtractor> bow;
};

/// Compute HSV color histogram.
struct HsvHistogramsCallback : Callback {
    HsvHistogramsCallback();

    arma::fvec operator()(const cv::Mat& image) const;

    DescriptorsType type() const { return vis::HSV; }
    size_t length() const { return hsv.getNumBins(); }

private:
    HsvExtractor hsv;
};

/// Compute both HOG bag-of-words and HSV color histogram.
struct CompositeCallback : Callback {
    CompositeCallback(const Vocabulary& v);

    arma::fvec operator()(const cv::Mat& image) const;

    DescriptorsType type() const { return vis::HOG_HSV; }
    size_t length() const { return hsv.length() + hog.length(); }

private:
    HogBagOfWordsCallback hog;
    HsvHistogramsCallback hsv;
};

/// Callback factory method.
Callback* getCallback(DescriptorsType type, const Vocabulary* vocabulary = nullptr);

} /* namespace vis */

#endif /* VIS_CALLBACKS_HPP */

