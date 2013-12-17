/**
 * @file descriptors.cpp
 * @brief Wrapper for descriptors.
 * @author Paolo D'Apice
 */

#include "vis/descriptors.hpp"
#include "vis/callbacks.hpp"
#include "vis/serialization.hpp"

namespace vis {

namespace fs = boost::filesystem;

Descriptors::Descriptors() {}

Descriptors::~Descriptors() {}

void
Descriptors::compute(const std::string& category,
        const std::vector<boost::filesystem::path>& files,
        const Callback& cb, ColorMode mode, ProgressHandler handler) {
    this->category = category;
    this->type = cb.type();
    vis::extract(files, descriptors, cb, mode, handler);
}

Descriptors*
Descriptors::load(const fs::path& file) {
    return vis::load<Descriptors, BinarySerializer>(file);
}

void
Descriptors::save(const fs::path& file) const {
    vis::save<Descriptors, BinarySerializer>(file, *this);
}

} /* namespace vis */

