/**
 * @file detail/extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_EXTRACT_IMPL_HPP
#define VIS_DETAIL_EXTRACT_IMPL_HPP

#include "../standardize.hpp"

#include <opencv2/highgui/highgui.hpp>

namespace vis {

template <typename Callback>
void
extract(const std::vector<boost::filesystem::path>& names,
        arma::fmat& output,
        const Callback& cb,
        ColorMode mode,
        ProgressHandler progress) {

    // TODO openmp parfor

    for (int i = 0; i < names.size(); ++i) {
        if (progress) progress(i);

        cv::Mat input = cv::imread(names[i].string(),
                                   mode == ColorMode::GRAYSCALE ? CV_LOAD_IMAGE_GRAYSCALE
                                                                : CV_LOAD_IMAGE_COLOR);
        BOOST_ASSERT(input.channels() == (mode == ColorMode::GRAYSCALE ? 1 : 3));

        cv::Mat image = standardizeImage(input);
        arma::fmat m = cb(image);
        output = arma::join_rows(output, m); // XXX slow
    }
}

} /* namespace vis */

#endif /* VIS_DETAIL_EXTRACT_IMPL_HPP */

