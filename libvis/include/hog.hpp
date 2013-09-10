/**
 * @file hog.hpp
 * @brief HOG wrapper
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
    HogDescriptors(vl_size width, vl_size height, vl_size dimension);

    ~HogDescriptors();

    /// Convert to OpenCV Mat.
    cv::Mat toMat() const;

#ifndef BOOST_TEST_MODULE
private:
#endif
    const vl_size width;
    const vl_size height;
    const vl_size dimension;
    float* data;

    friend class HogExtractor;
};

/// @brief Extracts HOG.
class HogExtractor : private boost::noncopyable {
public:
    HogExtractor(int cellSize = 8, int numOrientations = 9);

    ~HogExtractor();

    /// Extracts HOG from a gray-scale single-precision image.
    HogDescriptors extract(const cv::Mat& image);

    /// Render HOG descriptors to a glyph image.
    cv::Mat render(const HogDescriptors& descriptors) const;

private:
    const int cellSize;
    const int numOrientations;
    VlHog* hog;
};

} /* namespace vis */

#endif /* VIS_HOG_HPP */

