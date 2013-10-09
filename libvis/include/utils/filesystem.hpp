/**
 * @file utils/filesystem.hpp
 * @brief Filesystem utility functions
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_FILESYSTEM_HPP
#define VIS_UTILS_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <functional>
#include <string>
#include <vector>

namespace vis {

/// Match image file paths.
struct has_image_extension {
    bool operator()(const boost::filesystem::path& p) {
        const boost::filesystem::path ext = p.extension();
        return ext == ".png" || ext == ".jpg";
    }
};

/// Get image files in given directory.
std::vector<boost::filesystem::path>
getImageFiles(const boost::filesystem::path& dir) {
    std::vector<boost::filesystem::path> files;

    boost::filesystem::directory_iterator
            it = boost::filesystem::directory_iterator(dir),
            end = boost::filesystem::directory_iterator();

    std::copy(boost::make_filter_iterator<has_image_extension>(it, end),
              boost::make_filter_iterator<has_image_extension>(end, end),
              std::back_inserter(files));

    return files;
}

} /* namespace vis */

#endif /* VIS_UTILS_FILESYSTEM_HPP */

