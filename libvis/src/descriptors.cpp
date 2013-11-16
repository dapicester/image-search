/**
 * @file descriptors.cpp
 * @brief Wrapper for descriptors.
 * @author Paolo D'Apice
 */

#include "vis/descriptors.hpp"
#include "vis/serialization.hpp"

namespace vis {

namespace fs = boost::filesystem;

Descriptors::Descriptors() {}

Descriptors::~Descriptors() {}

Descriptors*
Descriptors::load(const fs::path& file) {
    return vis::load<Descriptors, BinarySerializer>(file);
}

void
Descriptors::save(const fs::path& file) const {
    vis::save<Descriptors, BinarySerializer>(file, *this);
}

} /* namespace vis */

