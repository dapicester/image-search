/**
 * @file utils/filesystem.hpp
 * @brief Filesystem utility functions
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_FILESYSTEM_HPP
#define VIS_UTILS_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <string>
#include <vector>

namespace vis {

namespace fs = boost::filesystem;

/// Match image file paths.
struct has_image_extension {
    bool operator()(const fs::path& p) {
        const fs::path ext = p.extension();
        return ext == ".png" || ext == ".jpg";
    }
};

/// Get image files in given directory.
std::vector<fs::path>
getImageFiles(const fs::path& dir) {
    std::vector<fs::path> files;

    fs::directory_iterator it = fs::directory_iterator(dir),
                           end = fs::directory_iterator();

    std::copy(boost::make_filter_iterator<has_image_extension>(it, end),
              boost::make_filter_iterator<has_image_extension>(end, end),
              std::back_inserter(files));

    return files;
}

} /* namespace vis */

#endif /* VIS_UTILS_FILESYSTEM_HPP */

