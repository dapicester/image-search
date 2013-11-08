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
colon[[deprecated]](const cv::Mat& in, const cv::Mat& indices, ColonDimension dim) {
    BOOST_ASSERT_MSG((indices.rows == 1 && indices.cols > 1)
                 xor (indices.cols == 1 && indices.rows > 1),
                 "indices is not a column vector");

    const int length = std::max(indices.rows, indices.cols);
    cv::Mat out;

    auto it = indices.begin<T>(), end = indices.end<T>();
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
colon[[deprecated]](const cv::Mat& in) {
    return in.reshape(0, 1);
}

/**
 * @brief Equivalent of the Matlab linspace function
 * Original code taken from:
 * http://dsj23.wordpress.com/2013/02/13/matlab-linspace-function-written-in-c/
 */
// TODO move to hsv_aux
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
    auto it = out.begin<T>(), end = out.end<T>();
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

template <typename T>
std::vector<T>
subset(const std::vector<T>& in, size_t num, SubsetMode mode) {
    size_t m = in.size();
    size_t n = std::min(m, num);

    std::vector<T> out;
    cv::Mat idx;
    switch (mode) {
    case UNIFORM:
        idx = linspace<int>(0, m-1, n);
        idx = round<int>(idx);
        break;
    default:
        // FIXME
        throw "not yet implemented!";
    }

    for (auto it = idx.begin<int>(); it != idx.end<int>(); ++it) {
        out.push_back(in[*it]);
    }
    BOOST_ASSERT(out.size() == n);

    return out;
}

/**
 * @brief Equivalent of Matlab reshape function.
 */
inline
cv::Mat
reshape[[deprecated]](const cv::Mat& in, size_t cols) {
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
static
cv::Mat
hist[[deprecated]](const cv::Mat& in, int numbins, NormalizeMode normalize = NONE) {
    BOOST_ASSERT_MSG(in.type() == cv::DataType<float>::type, "Support only float data type");

    double min, max;
    cv::minMaxIdx(in, &min, &max);
    float range[] = { static_cast<float>(min), static_cast<float>(max + 0.1)}; // XXX verify that range is correct
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

} /* namespace vis */

#endif /* VIS_UTILS_PORTED_HPP */

