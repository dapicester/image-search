/**
 * @file extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_EXTRACT_HPP
#define VIS_EXTRACT_HPP

#include "standardize.hpp"
#include <armadillo>
#include <boost/filesystem/path.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <functional>
#include <vector>

namespace vis {

enum class ColorMode {
    COLORS,
    GRAYSCALE
};

typedef std::function<void (int)> ProgressHandler;

/**
 * @brief Extract descriptors on a list of images.
 * @param names List of full pathnames to images
 * @param[out] data Descriptors.
 * @param cv Callback computing descriptors.
 * @param mode Color type of loaded images.
 * @param handler Callback to monitor extraction progress (e.g. adjust a progress counter).
 */
template <typename Callback>
void
extract(const std::vector<boost::filesystem::path>& names,
        arma::fmat& data,
        const Callback& cb,
        ColorMode mode = ColorMode::COLORS,
        ProgressHandler handler = NULL) {
    for (int i = 0; i < names.size(); ++i) {
        if (handler) handler(i);

        cv::Mat input = cv::imread(names[i].string(),
                                   mode == ColorMode::GRAYSCALE ? CV_LOAD_IMAGE_GRAYSCALE
                                                                : CV_LOAD_IMAGE_COLOR);
        BOOST_ASSERT(input.channels() == (mode == ColorMode::GRAYSCALE ? 1 : 3));

        cv::Mat image = standardizeImage(input);
        arma::fmat m = cb(image);
        data = arma::join_rows(data, m); // XXX slow
    }
}

} /* namespace vis */

#endif /* VIS_EXTRACT_HPP */

