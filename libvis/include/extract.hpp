/**
 * @file extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_EXTRACT_HPP
#define VIS_EXTRACT_HPP

#include <armadillo>
#include <boost/filesystem/path.hpp>
#include <vector>

namespace vis {

enum LoadImage {
    COLORS,
    GRAYSCALE
};

/**
 * @brief Extract descriptors on a list of images.
 * @param names List of full pathnames to images
 * @param output Descriptors
 * @param cv Callback computing descriptors
 */
template <typename Callback>
void
extract(const std::vector<boost::filesystem::path>& names,
        arma::fmat& output,
        const Callback& cb,
        LoadImage flag = COLORS);

} /* namespace vis */

#include "detail/extract_impl.hpp"

#endif /* VIS_EXTRACT_HPP */

