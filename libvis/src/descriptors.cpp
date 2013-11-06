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
    std::cout << "Loading descriptors from file " << file << " ... ";
    Descriptors* descriptors = vis::load<Descriptors, BinarySerializer>(file);
    std::cout << "done" << std::endl;
    return descriptors;
}

void
Descriptors::save(const fs::path& file) const {
    std::cout << "Saving descriptors to file " << file << " ... ";
    vis::save<Descriptors, BinarySerializer>(file, *this);
    std::cout << "done" << std::endl;
}

} /* namespace vis */

