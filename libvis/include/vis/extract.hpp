/**
 * @file extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_EXTRACT_HPP
#define VIS_EXTRACT_HPP

#include <armadillo>
#include <boost/filesystem/path.hpp>
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
        ProgressHandler handler = [](int){});

} /* namespace vis */

#include "detail/extract_impl.hpp"

#endif /* VIS_EXTRACT_HPP */

