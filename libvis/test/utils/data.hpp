/**
 * @file utils/data.hpp
 * @brief Test sample data
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_DATA_HPP
#define VIS_TEST_UTILS_DATA_HPP

#include "random.hpp"
#include <opencv2/core/core.hpp>

namespace vis {

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

} /* namespace vis */

#endif /* VIS_TEST_UTILS_DATA_HPP */

