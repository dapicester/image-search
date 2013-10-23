/**
 * @file utils.hpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#ifndef DEMO_UTILS_HPP
#define DEMO_UTILS_HPP

#include <QString>
#include <boost/filesystem/path.hpp>
#include <vector>

class QProgressDialog;
class QWidget;

/// Get QString string from std::string.
inline QString
str(const std::string string) {
    return QString(string.c_str());
}

/// Get QString string from boost::filesystem::path.
inline QString
str(const boost::filesystem::path& path) {
    return QString(path.string().c_str());
}

/// Load file content.
std::vector<std::string>
loadNames(const boost::filesystem::path& file);

/// Load file names from file.
std::vector<boost::filesystem::path>
loadNames(const boost::filesystem::path& file, const boost::filesystem::path& prefix);

/// Get the path to the text file containing images for the given category.
/// @return path to \c dataDir/category.txt
boost::filesystem::path
categoryFile(const boost::filesystem::path& dataDir, const QString& category);

/// Get the path to the directory containing images for the given category.
/// @return path to \c dataDir/category/
boost::filesystem::path
categoryDir(const boost::filesystem::path& dataDir, const QString& category);

/// Get the path to the vocabulary file for the given category.
/// @return path to \c dataDir/vocabulary_category.dgz
boost::filesystem::path
vocabularyFile(const boost::filesystem::path& dataDir, const QString& category);

/// Get the path to the descriptors file for the given category.
/// @return path to \c dataDir/descriptors_type_category.dgz
boost::filesystem::path
descriptorsFile(const boost::filesystem::path& dataDir, const QString& category, const QString& type);

/// Get the path to the index file for the given category.
/// @return path to \c dataDir/index_type_category.dgz
boost::filesystem::path
indexFile(const boost::filesystem::path& dataDir, const QString& category, const QString& type);

/// Display a confirmation message box.
bool confirmMessageBox(const QString& message);

/// Progress dialog box.
QProgressDialog* progressDialog(const QString& title, QWidget* parent, int min, int max);

/// Operator << for std::vector.
template <typename T>
std::ostream&
operator<<(std::ostream& os, std::vector<T> v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, "\n"));
    return os;
}

#endif /* DEMO_UTILS_HPP */

