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
#include <opencv2/imgproc/imgproc.hpp>

namespace vis {

// TODO move non-template implementations to cpp file

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
 * @brief Matlab colon operator returning all elements as a single column.
 */
inline
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

    cv::Mat out, idx;
    switch (mode) {
    case UNIFORM:
        idx = linspace<T>(0, m-1, n);
        idx = round<T>(idx);
        out = colon<T>(in, idx, COLUMNS);
        break;
    default:
        // FIXME
        throw "not yet implemented!";
    }
    BOOST_ASSERT(out.cols == n);
    return out;
}

/**
 * @brief Equivalent of Matlab reshape function.
 */
inline
cv::Mat
reshape(const cv::Mat& in, size_t cols) {
    cv::Mat out = in.t();
    return out.reshape(0, cols).t();
}

enum NormalizeMode {
    NONE,
    SUM1,
    RANGE1,
};

/**
 * @brief Equivalent of Matlab hist function.
 * Works only with floats!
 */
inline
cv::Mat
hist(const cv::Mat& in, int numbins, NormalizeMode normalize = NONE) {
    BOOST_ASSERT_MSG(in.type() == cv::DataType<float>::type, "Support only float data type");

    double min, max;
    cv::minMaxIdx(in, &min, &max);
    float range[] = { min, max + 0.1}; // XXX verify that range is correct
    const float* histRange = { range };

    cv::Mat histogram;
    cv::calcHist(&in, 1, 0, cv::Mat(), histogram, 1, &numbins, &histRange, true, false);

    switch (normalize) {
    case RANGE1:
        cv::normalize(histogram, histogram, 0, 1, cv::NORM_MINMAX);
        break;
    case SUM1:
        cv::normalize(histogram, histogram, 1, 0, cv::NORM_L1);
        break;
    default:
        break;
    }

    return histogram;
}

/**
 * @brief Equivalent of Matlab imquantize function.
 */
template <typename T>
cv::Mat
imquantize(const cv::Mat& in, const cv::Mat& levels) {
    BOOST_ASSERT_MSG(cv::DataType<T>::type == in.type(), "input is not of type T");
    BOOST_ASSERT_MSG(cv::DataType<T>::type == levels.type(), "levels is not of type T");

    size_t N = levels.total();
    cv::Mat index(in.size(), in.type(), cv::Scalar::all(1));

    for (int i = 0; i < N; i++) {
        cv::Mat temp = (in > levels.at<T>(i)) / 255;
        temp = temp.t(); // Matlab store images by columns, OpenCV by rows
        temp.convertTo(temp, cv::DataType<T>::type);
        index += temp;
    }

    return index;
}

/**
 * @brief Equivalent of Matlab ind2sub for a 3-dimensional input.
 */
template <typename T>
cv::Vec<T,3>
ind2sub(const cv::Vec<T,3>& size, T index) {
    BOOST_ASSERT_MSG(index >=0 and index < size[0]*size[1]*size[2], "out of range");

    // TODO refactor this

    cv::Vec<T,3> out;
    cv::Vec<T,3> k(1, size[0], size[0] * size[1]);

    T vi, vj;
    T n = index + 1; // 0-base index
    for (int i = 2; i >= 0; i--) {
        vi = (n - 1) % k[i] + 1;
        vj = (n - vi) / k[i] + 1;

        n = vi;
        out(i) = vj - 1; // 0-base index

        BOOST_ASSERT(n >= 0);
        BOOST_ASSERT(out(i) >= 0);
    }

    return out;
}

} /* namespace vis */

#endif /* VIS_UTILS_PORTED_HPP */

