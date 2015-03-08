/**
 * @file extract.hpp
 * @brief Descriptors and histograms computation
 * @author Paolo D'Apice
 */

#ifndef VIS_EXTRACT_HPP
#define VIS_EXTRACT_HPP

#include <armadillo>
#include <boost/filesystem/path.hpp>
#include <opencv2/core/core.hpp>
#include <functional>
#include <vector>

namespace vis {

struct Callback;
template <typename E> struct VocabularyCallback;

enum class ColorMode {
    COLORS,
    GRAYSCALE
};

typedef std::function<void (int)> ProgressHandler;

cv::Mat
readStandardized(const std::string& name, ColorMode mode);

/**
 * @brief Extract descriptor vectors on a list of images.
 * @param names List of full pathnames to images
 * @param[out] data Descriptors.
 * @param cv Callback computing descriptors.
 * @param mode Color type of loaded images.
 * @param handler Callback to monitor extraction progress (e.g. adjust a progress counter).
 */
void
extract(const std::vector<boost::filesystem::path>& names,
        arma::fmat& data,
        const Callback& cb,
        ColorMode mode = ColorMode::COLORS,
        ProgressHandler handler = NULL);

/**
 * @brief Extract descriptor matrices on a list of images.
 * @param names List of full pathnames to images
 * @param[out] data Descriptors.
 * @param cv Callback computing descriptors.
 * @param mode Color type of loaded images.
 * @param handler Callback to monitor extraction progress (e.g. adjust a progress counter).
 */
// TODO move this into vocabulary
template <typename Extractor>
void
extract_mat(const std::vector<boost::filesystem::path>& names,
        arma::fmat& data,
        const VocabularyCallback<Extractor>& cb,
        ColorMode mode = ColorMode::COLORS,
        ProgressHandler handler = NULL) {

    std::vector<arma::fmat> temp(names.size());
    for (int i = 0; i < names.size(); ++i) {
        if (handler) handler(i);

        cv::Mat image = readStandardized(names[i].string(), mode);
        temp[i] = cb(image);
    }

    for (int i = 0; i < names.size(); ++i) {
        data = arma::join_rows(data, temp[i]); // XXX this is slow
    }
}

} /* namespace vis */

#endif /* VIS_EXTRACT_HPP */

