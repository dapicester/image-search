/**
 * @file utils/filesystem.hpp
 * @brief Filesystem utility functions
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_FILESYSTEM_HPP
#define VIS_UTILS_FILESYSTEM_HPP

#include <boost/filesystem/path.hpp>
#include <vector>

namespace vis {

/// Get image files in given directory.
std::vector<boost::filesystem::path>
getImageFiles(const boost::filesystem::path& dir);

} /* namespace vis */

#endif /* VIS_UTILS_FILESYSTEM_HPP */

