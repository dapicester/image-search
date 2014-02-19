/**
 * @file recompute_main.cpp
 * @brief Utility for recomputing descriptors and indexes (executable).
 * @author Paolo D'Apice
 */

#include "directories.hpp"
#include "recompute.hpp"

#include <boost/filesystem.hpp>
#include <vis/descriptors_type.hpp>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    // available categories/types
    std::vector<std::string> TYPES = { "color", "shape", "combined" }; // TODO use DescriptorsType

    std::string category, type;

    switch (argc) {
    case 3:
        type = argv[2];
        try {
            vis::toDescriptorsType(type);
        } catch (...) {
            std::cout << "Wrong type" << std::endl;
            return 1;
        }
    case 2:
        category = argv[1];
        break;
    default:
        std::cout << "Wrong number of arguments" << std::endl;
        std::cout << "usage: " << argv[0] << " category [type]" << std::endl;
        return 1;
    }

    boost::filesystem::path cat = boost::filesystem::path(DATA_DIR)/category;
    if (not boost::filesystem::is_directory(cat)) {
        std::cout << "Wrong category" << std::endl;
        return 1;
    }

    Recompute worker(DATA_DIR);
    if (type.empty()) {
        // no type specified, recomputing for all
        for (auto t : TYPES) {
            worker.recompute(category, t);
        }
    } else {
        // recomputing only for specified type
        worker.recompute(category, type);
    }
}

