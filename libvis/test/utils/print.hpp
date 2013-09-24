/**
 * @file utils/print.hpp
 * @brief Utility functions for printing
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_PRINT_HPP
#define VIS_TEST_UTILS_PRINT_HPP

#include <iostream>
#include <vector>
#include <cstdio>

namespace vis {

#if 0
/// Print matrix stored into an array
template <typename T>
void
print(const T* data, size_t len) {
    printf("[");
    for (int i = 0; i < len; i += 2) {
        printf("%f ", data[i]);
    }
    printf("\n ");
    for (int i = 1; i < len; i += 2) {
        printf("%f ", data[i]);
    }
    printf("\b]\n");
}

/// Print vector stored into an array
template <typename T>
void
printv(const T* data, size_t len) {
    printf("[");
    for (int i = 0; i < len; i++) {
        printf("%f, ", data[i]);
    }
    printf("\b\b]\n");
}
#endif

/// Operator << for vector
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

/// Print vector to cout
template <typename T>
void
print(const std::vector<T>& collection) {
    std::copy(collection.begin(), collection.end(),
              std::ostream_iterator<T>(std::cout, "\n"));
}

} /* namespace vis */

#endif /* VIS_TEST_UTILS_PRINT_HPP */

