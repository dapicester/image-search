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

/// Get the path to the vocabulary file for the given category.
/// @return path to \c dataDir/vocabulary_category.dgz
boost::filesystem::path
vocabularyFile(const boost::filesystem::path& dataDir, const std::string& category);

/// Get the path to the descriptors file for the given category and query type.
/// @return path to \c dataDir/descriptors_category_type.dgz
boost::filesystem::path
descriptorsFile(const boost::filesystem::path& dataDir,
        const std::string& category, const std::string& type);

/// Get the path to the index file for the given category.
/// @return path to \c dataDir/index_category_type.dgz
boost::filesystem::path
indexFile(const boost::filesystem::path& dataDir,
        const std::string& category, const std::string& type);

} /* namespace vis */

#endif /* VIS_UTILS_FILESYSTEM_HPP */

