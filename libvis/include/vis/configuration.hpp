/**
 * @file configuration.hpp
 * @brief Image search configuration.
 * @author Paolo D'Apice
 */

#ifndef VIS_CONFIGURATION_HPP
#define VIS_CONFIGURATION_HPP

#include <string>
#include <vector>

namespace vis {
namespace config {

/// Image search category configuration.
struct Category {
    std::string name;
    std::string dir;
    std::vector<std::string> type;
};

/// Image server configuration.
struct Configuration {
    std::vector<Category> categories;
};

/// Load configuration from file.
Configuration loadConfiguration(const std::string& file);

/// Dump configuration to stream.
void dumpConfiguration(std::ostream& os, const Configuration& conf);

} // namespace config
} // namespace vis

#endif // VIS_CONFIGURATION_HPP

