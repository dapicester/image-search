/**
 * @file descriptors.cpp
 * @brief Wrapper for descriptors.
 * @author Paolo D'Apice
 */

#include "descriptors.hpp"
#include "serialization.hpp"

namespace vis {

namespace fs = boost::filesystem;

Descriptors::Descriptors() {}
Descriptors::~Descriptors() {}

Descriptors*
Descriptors::load(const fs::path& file) {
    printf("Loading descriptors from file %s ... ", file.string().c_str());
    Descriptors* descriptors = vis::load<Descriptors, BinarySerializer>(file);
    printf("done\n");
    return descriptors;
}

void
Descriptors::save(const fs::path& file) const {
    printf("Saving descriptors to file %s ... ", file.string().c_str());
    vis::save<Descriptors, BinarySerializer>(file, *this);
    printf("done\n");
}

} /* namespace vis */

