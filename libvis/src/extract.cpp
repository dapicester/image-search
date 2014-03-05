/**
 * @file extract.cpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#include "vis/callbacks.hpp"
#include "vis/extract.hpp"
#include "vis/standardize.hpp"
#include <opencv2/highgui/highgui.hpp>

namespace vis {

/// @brief Read and standardize an image from file.
cv::Mat
readStandardized(const std::string& name, ColorMode mode) {
    cv::Mat input = cv::imread(name,
                               mode == ColorMode::GRAYSCALE ? CV_LOAD_IMAGE_GRAYSCALE
                                                            : CV_LOAD_IMAGE_COLOR);
    BOOST_ASSERT(input.channels() == (mode == ColorMode::GRAYSCALE ? 1 : 3));

    return standardizeImage(input);
}

void
extract(const std::vector<boost::filesystem::path>& names,
        arma::fmat& data,
        const Callback& cb,
        ColorMode mode,
        ProgressHandler handler) {

    data.set_size(cb.length(), names.size());
    for (int i = 0; i < names.size(); ++i) {
        if (handler) handler(i);

        cv::Mat image = readStandardized(names[i].string(), mode);
        data.col(i) = cb(image);
    }
}

} /* namespace vis */

