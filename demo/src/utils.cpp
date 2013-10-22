/**
 * @file utils.cpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#include "utils.hpp"
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

