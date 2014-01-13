/**
 * @file configuration.cpp
 * @brief Image search configuration.
 * @author Paolo D'Apice
 */

#include "vis/configuration.hpp"

#include <yaml-cpp/yaml.h>

namespace vis {
namespace config {

Configuration
loadConfiguration(const std::string& file) {
    return YAML::LoadFile(file).as<Configuration>();
}

void
dumpConfiguration(std::ostream& os, const Configuration& conf) {
    YAML::Node node;
    node = conf;
    os << YAML::Dump(node);
}

} // namespace config
} // namespace vis

namespace YAML {

    template <>
    struct convert<vis::config::Category> {
        static Node encode(const vis::config::Category& category) {
            Node node;
            node["dir"] = category.dir;
            for (auto type : category.type) {
                node["type"].push_back(type);
            }
            return node;
        }
        static bool decode(const Node& node, vis::config::Category& category) {
            if (!node.IsMap()
                    or !node["dir"] or !node["type"])
                return false;

            category.dir = node["dir"].as<std::string>();
            category.type = node["type"].as<std::vector<std::string>>();
            return true;
        }
    };

    template <>
    struct convert<vis::config::Configuration> {
        static Node encode(const vis::config::Configuration& conf) {
            Node categories;
            for (auto cat : conf.categories) {
                categories[cat.name] = convert<vis::config::Category>::encode(cat);
            }
            return categories;
        }
        static bool decode(const Node& node, vis::config::Configuration& conf) {
            if (!node["categories"])
                return false;

            const Node& categories = node["categories"];
            if (!categories.IsMap())
                return false;

            for (auto pair : categories) {
                vis::config::Category cat = pair.second.as<vis::config::Category>();
                cat.name = pair.first.as<std::string>();
                conf.categories.push_back(cat);
            }

            return true;
        }
    };

} // namespace YAML

