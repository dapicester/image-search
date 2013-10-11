/**
 * @file utils/print.hpp
 * @brief Utility functions for printing
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_PRINT_HPP
#define VIS_UTILS_PRINT_HPP

#include <iostream>
#include <vector>

#define print(X) std::cout << #X << ": " << X << std::endl
#define printmat(X) std::cout << #X << ":\n" << X << std::endl

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

#endif /* VIS_UTILS_PRINT_HPP */

