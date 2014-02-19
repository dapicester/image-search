/**
 * @file utils/filesystem.cpp
 * @brief Filesystem utility functions
 * @author Paolo D'Apice
 */

#include "vis/utils/filesystem.hpp"

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <functional>
#include <fstream>
#include <string>
#include <vector>

namespace vis {

namespace fs = boost::filesystem;

static const std::string DATA_EXT = "dgz";
static const std::string TEXT_EXT = "txt";

/// Match image file paths.
struct has_image_extension {
    bool operator()(const fs::path& p) {
        const fs::path& ext = p.extension();
        return ext == ".png" || ext == ".jpg";
    }
};

std::vector<fs::path>
getImageFiles(const fs::path& dir) {
    std::vector<fs::path> files;

    fs::directory_iterator it = fs::directory_iterator(dir);
    fs::directory_iterator end = fs::directory_iterator();

    std::copy(boost::make_filter_iterator<has_image_extension>(it, end),
              boost::make_filter_iterator<has_image_extension>(end, end),
              std::back_inserter(files));

    return files;
}

std::vector<std::string>
loadNames(const fs::path& file) {
    std::vector<std::string> names;
    std::ifstream input(file.string());
    std::copy(std::istream_iterator<std::string>(input),
              std::istream_iterator<std::string>(),
              std::back_inserter(names));
    return names;
}

std::vector<fs::path>
loadNames(const fs::path& file, const fs::path& prefix) {
    std::vector<std::string> lines = loadNames(file);
    std::vector<fs::path> names;
    std::for_each (lines.begin(), lines.end(), [&](const std::string& line) {
        names.push_back(prefix / line);
    });
    return names;
}

fs::path
categoryFile(const fs::path& dataDir, const std::string& category) {
    boost::format format("%1%.%2%");
    format % category % TEXT_EXT;
    return dataDir / format.str();
}

fs::path
vocabularyFile(const fs::path& dataDir, const std::string& category) {
    boost::format format("vocabulary_%1%.%2%");
    format % category % DATA_EXT;
    return dataDir / format.str();
}

fs::path
descriptorsFile(const fs::path& dataDir, const std::string& category, const std::string& type) {
    boost::format format("descriptors_%1%_%2%.%3%");
    format % category % type % DATA_EXT;
    return dataDir / format.str();
}

fs::path
indexFile(const fs::path& dataDir, const std::string& category, const std::string& type) {
    boost::format format("index_%1%_%2%.%3%");
    format % category % type % DATA_EXT;
    return dataDir / format.str();
}

void
toStrings(const std::vector<fs::path>& paths, std::vector<std::string>& strings) {
    for (const fs::path& p : paths) {
        strings.push_back(p.string());
    }
}

} /* namespace vis */

