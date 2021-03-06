/**
 * @file utils.hpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#ifndef DEMO_UTILS_HPP
#define DEMO_UTILS_HPP

#include <vis/descriptors_type.hpp>
#include <vis/extract.hpp>

#include <boost/filesystem/path.hpp>
#include <vector>

#include <QMessageBox>
#include <QString>

class QProgressDialog;
class QWidget;

namespace vis {
    class Descriptors;
    class Vocabulary;
}

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

/// Decode type to string.
inline QString
decodeType(vis::DescriptorsType type) {
    switch (type) {
    case vis::HOG: return "shape";
    case vis::HSV: return "color";
    case vis::HOG_HSV: return "combined";
    }
}

/// Get query images for the given category.
std::vector<boost::filesystem::path>
queryNames(const std::vector<boost::filesystem::path>& all, const QString& category);

/// Get the path to the query file for the given category and query type.
/// @return path to \c dataDir/query_category_type.dgz
boost::filesystem::path
queryFile(const boost::filesystem::path& dataDir, const QString& category, const QString& type);

void
extractDescriptors(const QString& category, const QString& queryType,
        const std::vector<boost::filesystem::path>& names,
        vis::Descriptors* descriptors, vis::Vocabulary* vocabulary,
        vis::ProgressHandler handler = NULL);

/// Display a confirmation message box.
bool confirmMessageBox(const QString& message);

/// Display a message box.
void messageBox(const QString& message, QMessageBox::Icon icon = QMessageBox::NoIcon);

/// Progress dialog box.
QProgressDialog* progressDialog(const QString& title, QWidget* parent, int max);

/// Operator << for std::vector.
template <typename T>
std::ostream&
operator<<(std::ostream& os, std::vector<T> v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, "\n"));
    return os;
}

#endif /* DEMO_UTILS_HPP */

