/**
 * @file utils.cpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#include "utils.hpp"
#include <boost/foreach.hpp>
#include <fstream>

namespace fs = boost::filesystem;

using std::string;
using std::vector;

vector<string>
loadNames(const fs::path& file) {
    vector<string> names;
    std::ifstream input(file.string().c_str());
    std::copy(std::istream_iterator<string>(input),
              std::istream_iterator<string>(),
              std::back_inserter(names));
    return names;
}

vector<fs::path>
loadNames(const fs::path& file, const fs::path& prefix) {
    vector<string> lines = loadNames(file);
    vector<fs::path> names;
    BOOST_FOREACH(const string& line, lines) {
        names.push_back(prefix / line);
    }
    return names;
}

fs::path
vocabularyFile(const fs::path& dataDir, const QString& category) {
    QString file = category;
    file.prepend("vocabulary_");
    file.append(".dgz");
    return dataDir / file.toStdString();
}

fs::path
indexFile(const fs::path& dataDir, const QString& category, const QString& type) {
    QString file = category;
    file.append("_");
    file.append(type);
    file.append("_index.dgz");
    return dataDir / file.toStdString();
}

