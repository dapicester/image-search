/**
 * @file hsv.hpp
 * @brief HSV color histogram
 * @author Paolo D'Apice
 */

#ifndef VIS_HSV_HPP
#define VIS_HSV_HPP

#include <boost/noncopyable.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

namespace hsv {
    static const cv::Vec3i levels = cv::Vec3i(18, 3, 3); ///< Default quantization levels.
}

/// @brief Extracts HSV color histogram.
class HsvExtractor : private boost::noncopyable {
public:
    HsvExtractor(const cv::Vec3i& levels = hsv::levels, bool medfilt = true);
    ~HsvExtractor();

    /// @brief Extracts HSV color histogram from a color single-precision image.
    cv::Mat extract(const cv::Mat& image, bool normalize = true, cv::OutputArray& quantized = cv::noArray()) const;

    /// @brief Render the histogram to a displayable image.
    cv::Mat render(const cv::Mat& histogram) const;

    size_t getNumBins() const;

private:
    const cv::Vec3i levels;
    const bool medfilt;
};

/// Convert a BGR color single-precision image to HSV colorspace.
cv::Mat toHsv(const cv::Mat& image);

/// Convert a HSV color single-precision image to BGR colorspace.
cv::Mat toBgr(const cv::Mat& image, const cv::Vec3i& levels = cv::Vec3i());

/// Quantize image channels to given levels.
cv::Mat quantize(const cv::Mat& image, const cv::Vec3i& levels);

} /* namespace vis */

#endif /* VIS_HSV_HPP */

