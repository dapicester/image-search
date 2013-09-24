/**
 * @file test_utils.hpp
 * @brief Test utilities
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_HPP
#define VIS_TEST_UTILS_HPP

#include "traits.hpp"
#include <opencv2/core/core.hpp>
extern "C" {
#include <vl/random.h>
}

/// @return true if equals
bool
equals(const cv::Mat& a, const cv::Mat& b) {
    if (a.size() != b.size()) return false;
    cv::Mat diff = (a != b);
    return cv::countNonZero(diff) == 0 ? true : false;
}

template <typename T>
class Random {
public:
    Random() {
        rand = vl_get_rand();
    }

    T next() {
        switch (vis::VlType<T>::type) {
        case VL_TYPE_FLOAT:
        case VL_TYPE_DOUBLE:
            return static_cast<T>(vl_rand_real1(rand));
        case VL_TYPE_INT32:
            return static_cast<T>(vl_rand_int31(rand));
        }
    }

private:
    VlRand* rand;
};

/// @return A pointer to rows x columns random data
template <typename T>
T*
getTestDataPtr(int rows, int columns) {
    static Random<T> rand;
    T* data = new T[rows*columns];
    for (int i = 0; i < rows*columns; ++i) {
        data[i] = rand.next();
    }

    return data;
}

/// @return A vector to rows x columns random data
template <typename T>
std::vector<T>
getTestDataVec(int rows, int columns) {
    T* data = getTestDataPtr<T>(rows, columns);
    std::vector<T> vec(data, data + rows*columns);
    delete[] data;
    return vec;
}


/// @return A matrix with rows x columns random data
template <typename T>
cv::Mat
getTestData(int rows, int columns) {
    T* data = getTestDataPtr<T>(rows, columns);
    cv::Mat mat = cv::Mat(rows, columns, cv::DataType<T>::type, data).clone();
    delete[] data;
    return mat;
}

#endif /* VIS_TEST_UTILS_HPP */

