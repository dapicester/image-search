/**
 * @file utils/ported.hpp
 * @brief Collections of functions ported from Matlab.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_PORTED_HPP
#define VIS_UTILS_PORTED_HPP

#include <boost/assert.hpp>
#include <opencv2/core/core.hpp>

namespace vis {

// TODO costruire matrice con varargs

enum ColonDimension {
    ROWS,
    COLUMNS,
};

/**
 * @brief Matlab colon operator used to pick out selected columns.
 * @param in Input data
 * @param indices Row or column vector of indices
 * @return A new matrix containing only rows/columns subset of in
 */
cv::Mat colon(const cv::Mat& in, const cv::Mat& indices, ColonDimension dim) {
    BOOST_ASSERT_MSG((indices.rows == 1 && indices.cols > 1)
                 xor (indices.cols == 1 && indices.rows > 1),
                 "indices is not a column vector");

    const int sz = std::max(indices.rows, indices.cols);
    cv::Mat out;

    switch (dim) {
    case COLUMNS:
        out = cv::Mat(in.rows, sz, in.type());
        for (int i = 0; i < sz; ++i) {
            int index = indices.at<int>(i);
            cv::Mat col = in.col(index);
            col.copyTo(out.col(i));
        }
        break;
    case ROWS:
        out = cv::Mat(sz, in.cols, in.type());
        for (int i = 0; i < sz; ++i) {
            int index = indices.at<int>(i);
            cv::Mat row = in.row(index);
            row.copyTo(out.row(i));
        }
        break;
    }
    return out;
}

} /* namespace vis */

#endif /* VIS_UTILS_PORTED_HPP */

