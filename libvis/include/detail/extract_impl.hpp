/**
 * @file detail/extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_EXTRACT_IMPL_HPP
#define VIS_DETAIL_EXTRACT_IMPL_HPP

#include "standardize.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

namespace vis {

template <typename Callback>
void
extract(const std::vector<boost::filesystem::path>& names,
        arma::fmat& output,
        const Callback& cb,
        ColorMode mode) {

    // TODO openmp parfor
    size_t numImages = names.size();
    size_t i = 0;

    for (auto it = names.begin(); it != names.end(); ++it) {
        const std::string& name = it->string();

        // TODO control this verbosity
        std::cout << "  Extracting features from " << name
                  << " (" << i+1 << "/" << numImages << ")" << std::endl;

        cv::Mat input = cv::imread(name, mode == ColorMode::GRAYSCALE ? CV_LOAD_IMAGE_GRAYSCALE
                                                                      : CV_LOAD_IMAGE_COLOR);
        BOOST_ASSERT(input.channels() == (mode == ColorMode::GRAYSCALE ? 1 : 3));

        cv::Mat image = standardizeImage(input);
        arma::fmat m = cb(image);
        output = arma::join_rows(output, m); // XXX slow
        ++i;
    }
}

} /* namespace vis */

#endif /* VIS_DETAIL_EXTRACT_IMPL_HPP */

