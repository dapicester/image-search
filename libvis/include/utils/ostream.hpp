/**
 * @file utils/ostream.hpp
 * @brief Utilities for streaming objects.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_OSTREAM_HPP
#define VIS_UTILS_OSTREAM_HPP

#include "kdtree.hpp"
#include <ostream>

/// @brief Operator << for std::ostream and KDTreeNeighbor.
std::ostream&
operator<<(std::ostream& os, const vis::KDTreeNeighbor& n) {
    return os << "{" << n.index << "," << n.distance << "}";
}

/// @brief Operator << for std::ostream and KDTreeIndex.
std::ostream&
operator<<(std::ostream& os, const vis::KDTreeIndex& i) {
    return os << "{" << i.index << "}";
}

#endif /* VIS_UTILS_OSTREAM_HPP */

