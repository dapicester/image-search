/**
 * @file utils/data.hpp
 * @brief Test sample data
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_DATA_HPP
#define VIS_TEST_UTILS_DATA_HPP

#include "random.hpp"
#include <armadillo>
#include <opencv2/core/core.hpp>

namespace test {

/// @return A pointer to rows x columns random data
template <typename T>
T*
getTestDataPtr(int rows, int columns) {
    static vis::Random<T> rand;
    T* data = new T[rows*columns];
    for (int i = 0; i < rows*columns; ++i) {
        data[i] = rand.next();
    }
    return data;
}

/// @return A matrix with rows x columns uniform random data.
template <typename T>
arma::Mat<T>
testData(int rows, int columns) {
    return arma::randu<arma::Mat<T>>(rows, columns);
}

/// @return A matrix with rows x columns random data
template <typename T>
cv::Mat
getTestData[[deprecated]](int rows, int columns) {
    cv::Mat data(rows, columns, cv::DataType<T>::type);
    cv::randu(data, cv::Scalar(0), cv::Scalar(1));
    return data;
}

} /* namespace test */

#endif /* VIS_TEST_UTILS_DATA_HPP */

