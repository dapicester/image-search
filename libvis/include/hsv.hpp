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

typedef arma::ivec3 levels_t;

namespace hsv {
    static const levels_t levels = { 18, 3, 3 }; ///< Default quantization levels.
}

/// @brief Extracts HSV color histogram.
class HsvExtractor : private boost::noncopyable {
public:
    /// Constructor.
    HsvExtractor(const levels_t& levels = hsv::levels, bool medfilt = true);

    /// Default destructor.
    ~HsvExtractor();

    /// @brief Extracts HSV color histogram from a color single-precision image.
    arma::fmat extract(const cv::Mat& image, bool normalize = true, cv::OutputArray& quantized = cv::noArray()) const;

    /// @brief Render the histogram to a displayable image.
    cv::Mat render(const arma::fmat& histogram) const;

    /// @return The number of bins in the histogram.
    size_t getNumBins() const;

private:
    const levels_t levels;    ///< Quantization levels as passed to constructor.
    const levels_t hsvlevels; ///< Actual HSV quantization levels including gray levels.
    const bool medfilt;
};

} /* namespace vis */

#endif /* VIS_HSV_HPP */

