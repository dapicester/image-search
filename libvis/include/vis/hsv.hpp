/**
 * @file hsv.hpp
 * @brief HSV color histogram
 * @author Paolo D'Apice
 */

#ifndef VIS_HSV_HPP
#define VIS_HSV_HPP

#include <armadillo>
#include <boost/noncopyable.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

namespace hsv {
    typedef arma::ivec3 Levels; ///< Type alias for quantization levels.
}

/// @brief Extracts HSV color histogram.
class HsvExtractor : private boost::noncopyable {
public:
    /// Constructor.
    HsvExtractor(const hsv::Levels& levels = {18,3,3}, bool medfilt = true);

    /// Default destructor.
    ~HsvExtractor();

    /**
     * @brief Extracts HSV color histogram.
     * HSV color histogram are computed from an RGB color single-precision image.
     * @param image Input image.
     * @param normalize Whether normalize the histogram to the unit sum.
     * @param[out] quantized The color-quantized image.
     */
    arma::fvec extract(const cv::Mat& image, bool normalize = true,
                       cv::OutputArray& quantized = cv::noArray()) const;

    /// @brief Render the histogram to a displayable image.
    cv::Mat render(const arma::fmat& histogram) const;

    /// @return The number of bins in the histogram.
    size_t getNumBins() const;

private:
    const hsv::Levels levels;    ///< Quantization levels as passed to constructor.
    const hsv::Levels hsvlevels; ///< Actual HSV quantization levels including gray levels.
    const bool medfilt;
};

} /* namespace vis */

#endif /* VIS_HSV_HPP */

