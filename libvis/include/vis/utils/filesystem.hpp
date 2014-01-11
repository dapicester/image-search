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

/// Load file content.
std::vector<std::string>
loadNames(const boost::filesystem::path& file); // TODO rename into getFileNames

/// Load file names from file.
std::vector<boost::filesystem::path>
loadNames(const boost::filesystem::path& file, const boost::filesystem::path& prefix); // TODO rename into getFileNames

/// Get the path to the text file containing images for the given category.
/// @return path to \c dataDir/category.txt
boost::filesystem::path
categoryFile(const boost::filesystem::path& dataDir, const std::string& category);

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

/// Converts a vector of paths into a vector of strings.
void toStrings(const std::vector<boost::filesystem::path>& paths,
        std::vector<std::string>& strings);

} /* namespace vis */

#endif /* VIS_UTILS_FILESYSTEM_HPP */

