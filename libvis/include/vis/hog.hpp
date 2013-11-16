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
#include <armadillo>
#include <boost/noncopyable.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

/// @brief Extracts HOG.
class HogExtractor : private boost::noncopyable {
public:
    /// Constructor.
    HogExtractor(int cellSize = 8, int numOrientations = 8 /* TODO = 9 */);

    /// Destructor.
    ~HogExtractor();

    /// @brief Extracts HOG from a gray-scale single-precision image.
    arma::fmat extract(const cv::Mat& image) const;

    /// @brief Renders HOG descriptors to a glyph image.
    cv::Mat render(const arma::fmat& descriptors) const;

private:
    const int cellSize;
    const int numOrientations;
    VlHog* hog;
};

} /* namespace vis */

#endif /* VIS_HOG_HPP */

