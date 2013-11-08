/**
 * @file utils/print.hpp
 * @brief Utility functions for printing
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_PRINT_HPP
#define VIS_UTILS_PRINT_HPP

#include "utils/ostream.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>

#define printvar(X) std::cout << #X << ": " << X << std::endl
#define printmat(X) std::cout << #X << ":\n" << X << std::endl
#define println(X) std::cout << X << std::endl

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

/// Print multidimensional matrix (OpenCV does not support it)
static
void
printMat3(const cv::Mat& mat, const int* dims, bool showindices = false) {
    if (not showindices) std::cout << '[';
    for (int i = 0; i < dims[0]; i++) {
        for (int j = 0; j < dims[1]; j++) {
            for (int k = 0; k < dims[2]; k++) {
                std::cout << mat.at<int>(i,j,k) << ' ';
                if (showindices)
                    std::cout << "@(" << i << ',' << j << ',' << k << ")\n";
            }
        }
    }
    if (not showindices) std::cout << "\b]\n";
}

#endif /* VIS_UTILS_PRINT_HPP */

