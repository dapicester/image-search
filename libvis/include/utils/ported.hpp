/**
 * @file utils/ported.hpp
 * @brief Collections of functions ported from Matlab.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_PORTED_HPP
#define VIS_UTILS_PORTED_HPP

#include <boost/assert.hpp>
#include <boost/math/special_functions/round.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

// TODO costruire matrice con varargs

enum ColonDimension {
    ROWS,
    COLUMNS,
};

/**
 * @brief Matlab colon operator used to pick out selected columns.
 * @tparam T Input data type
 * @param in Input data
 * @param indices Row or column vector of indices
 * @return A new matrix containing only rows/columns subset of in
 */
template <typename T>
cv::Mat
colon(const cv::Mat& in, const cv::Mat& indices, ColonDimension dim) {
    BOOST_ASSERT_MSG((indices.rows == 1 && indices.cols > 1)
                 xor (indices.cols == 1 && indices.rows > 1),
                 "indices is not a column vector");

    const int length = std::max(indices.rows, indices.cols);
    cv::Mat out;

    cv::MatConstIterator_<T> it = indices.begin<T>(), end = indices.end<T>();
    // TODO: switch on channels too
    switch (dim) {
    case COLUMNS:
        out = cv::Mat(in.rows, length, in.type());
        for (int i = 0; it != end; ++i, ++it) {
            int index = static_cast<int>(*it);
            cv::Mat col = in.col(index);
            col.copyTo(out.col(i));
        }
        break;
    case ROWS:
        out = cv::Mat(length, in.cols, in.type());
        for (int i = 0; it != end; ++i, ++it) {
            int index = static_cast<int>(*it);
            cv::Mat row = in.row(index);
            row.copyTo(out.row(i));
        }
        break;
    }
    return out;
}

/**
 * @brief Matlab colon operator returningall elements as a single column.
 * @tparam T Input data type
 * @param in Input data
 */
template <typename T>
cv::Mat
colon(const cv::Mat& in) {
    return in.reshape(0, 1);
}

/**
 * @brief Equivalent of the Matlab linspace function
 * Original code taken from:
 * http://dsj23.wordpress.com/2013/02/13/matlab-linspace-function-written-in-c/
 */
template <typename T>
cv::Mat
linspace(T min, T max, size_t n) {
    cv::Mat result = cv::Mat_<T>(1,n);
    T* ptr = result.ptr<T>(0);
    const T range = max - min;
    for (size_t i = 0; i < n-1; i++) {
        ptr[i] = min + i*range/(n - 1);
    }
    ptr[n-1] = max;

    return result;
}

/// @brief Round to the nearest integer
template <typename T>
double round(T in) {
    return boost::math::round(in);
}

/// @brief Round to the nearest integer
template <typename T>
int iround(T in) {
    return boost::math::iround(in);
}

/**
 * @brief Equivalent of the Matlab round function
 */
template <typename T>
cv::Mat
round(const cv::Mat& in) {
    cv::Mat out = in.clone();
    cv::MatIterator_<T> it = out.begin<T>(), end = out.end<T>();
    for (; it != end; ++it) {
        *it = round(*it);
    }
    return out;
}

enum SubsetMode {
    BEGINNING,
    ENDING,
    RANDOM,
    UNIFORM,
    LARGEST,
    SMALLEST
};

/**
 * @brief Select a given number of columns
 */
template <typename T>
cv::Mat
colsubset(const cv::Mat& in, size_t num, SubsetMode mode) {
    size_t m = in.size().width;
    size_t n = std::min(m, num);

    cv::Mat out;
    switch (mode) {
    case UNIFORM: {
        cv::Mat ind = linspace<T>(0, m-1, n);
        ind = round<T>(ind);
        out = colon<T>(in, ind, COLUMNS);
        }
        break;
    default:
        // FIXME
        throw "not yet implemented!";
    }
    return out;
}

} /* namespace vis */

#endif /* VIS_UTILS_PORTED_HPP */

