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
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
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

/// @return A matrix with rows x columns random data
template <typename T>
cv::Mat
getTestData(int rows, int columns) {
    static Random<T> rand;

    cv::Mat data = cv::Mat::zeros(rows, columns, cv::DataType<T>::type);
    if (data.isContinuous()) {
        columns *= rows;
        rows = 1;
    }

    T* p;
    for(int i = 0; i < rows; i++) {
        p = data.ptr<T>(i);
        for(int j = 0; j < columns; j++) {
            T randomNum = rand.next();
            p[j] += randomNum;
        }
    }

    return data;
}

#endif /* VIS_TEST_UTILS_HPP */

