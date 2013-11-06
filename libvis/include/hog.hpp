/**
 * @file hog.hpp
 * @brief VlFeat HOG wrapper
 * @author Paolo D'Apice
 */

#ifndef VIS_HOG_HPP
#define VIS_HOG_HPP

extern "C" {
#include <vl/hog.h>
}
#include <boost/noncopyable.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

class HogExtractor;

/// @brief HOG descriptors data structure.
struct HogDescriptors {

    /// Constructor.
    HogDescriptors(vl_size width, vl_size height, vl_size dimension);

    /// Default destructor.
    ~HogDescriptors();

    /// @brief Convert to OpenCV Mat.
    cv::Mat toMat() const;

    const vl_size width;
    const vl_size height;
    const vl_size dimension;

private:
    float* data;

    friend class HogExtractor;
};

/// @brief Extracts HOG.
class HogExtractor : private boost::noncopyable {
public:
    /// Constructor.
    HogExtractor(int cellSize = 8, int numOrientations = 8); // TODO numOrientations = 9 ?

    /// Default destructor.
    ~HogExtractor();

    /// @brief Extracts HOG from a gray-scale single-precision image.
    HogDescriptors extract(const cv::Mat& image) const;

    /// @brief Renders HOG descriptors to a glyph image.
    cv::Mat render(const HogDescriptors& descriptors) const;

private:
    const int cellSize;
    const int numOrientations;
    VlHog* hog;
};

} /* namespace vis */

#endif /* VIS_HOG_HPP */

