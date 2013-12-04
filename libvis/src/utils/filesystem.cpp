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

} /* namespace vis */

