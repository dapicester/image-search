/**
 * @file configuration.cpp
 * @brief Server configuration.
 * @author Paolo D'Apice
 */

#include "configuration.hpp"

#include <yaml-cpp/yaml.h>

namespace vis {
namespace server {

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

} // namespace server
} // namespace vis

namespace YAML {

    template <>
    struct convert<vis::server::Category> {
        static Node encode(const vis::server::Category& category) {
            Node node;
            node["dir"] = category.dir;
            node["type"] = category.type;
            return node;
        }
        static bool decode(const Node& node, vis::server::Category& category) {
            if (!node.IsMap()
                    or !node["dir"] or !node["type"])
                return false;

            category.dir = node["dir"].as<std::string>();
            category.type = node["type"].as<std::string>();
            return true;
        }
    };

    template <>
    struct convert<vis::server::Configuration> {
        static Node encode(const vis::server::Configuration& conf) {
            Node categories;
            for (auto cat : conf.categories) {
                categories[cat.name] = convert<vis::server::Category>::encode(cat);
            }
            return categories;
        }
        static bool decode(const Node& node, vis::server::Configuration& conf) {
            if (!node["categories"])
                return false;

            const Node& categories = node["categories"];
            if (!categories.IsMap())
                return false;

            for (auto pair : categories) {
                vis::server::Category cat = pair.second.as<vis::server::Category>();
                cat.name = pair.first.as<std::string>();
                conf.categories.push_back(cat);
            }

            return true;
        }
    };

}


