/**
 * @file utils/print.hpp
 * @brief Utility functions for printing
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_PRINT_HPP
#define VIS_TEST_UTILS_PRINT_HPP

#include "kdtree.hpp"
#include <iostream>
#include <vector>

namespace vis {

/// Operator << for std::vector
template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    typename std::vector<T>::const_iterator it = v.begin(), end = v.end();
    while (it != end) {
        os << *it;
        it++;
        if (it != end) os << ", ";
    }
    os << "]";
    return os;
}

/// Operator << for KDTreeNeighbor
std::ostream&
operator<<(std::ostream& os, const KDTreeNeighbor& n) {
    os << "{" << n.index << "," << n.distance << "}";
    return os;
}

/// Operator << for KDTreeIndex
std::ostream&
operator<<(std::ostream& os, const KDTreeIndex& i) {
    os << "{" << i.index << "}";
    return os;
}

/// Print vector to cout
template <typename T>
void
print(const std::vector<T>& collection) {
    std::copy(collection.begin(), collection.end(),
              std::ostream_iterator<T>(std::cout, "\n"));
}

} /* namespace vis */

#endif /* VIS_TEST_UTILS_PRINT_HPP */

