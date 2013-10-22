/**
 * @file utils.hpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#ifndef DEMO_UTILS_HPP
#define DEMO_UTILS_HPP

#include <boost/filesystem/path.hpp>
#include <vector>

/// Load file content.
std::vector<std::string>
loadNames(const boost::filesystem::path& file);

template <typename T>
std::ostream&
operator<<(std::ostream& os, std::vector<T> v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, "\n"));
    return os;
}

#endif /* DEMO_UTILS_HPP */

