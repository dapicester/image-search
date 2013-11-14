/**
 * @file detail/serialization_matrix.hpp
 * @brief Armadillo and OpenCV matrix serialization using Boost Serialization.
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAILS_SERIALIZATION_MATRIX_HPP
#define VIS_DETAILS_SERIALIZATION_MATRIX_HPP

#include <armadillo>
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/array.hpp>
#include <opencv2/core/core.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat)

namespace boost { namespace serialization {

template <typename Archive, typename T>
void
serialize(Archive & ar, arma::Mat<T>& mat, const unsigned int version) {
    split_free(ar, mat, version);
}

/// @brief Boost serialization for @a arma::Mat.
template <typename OutputArchive, typename T>
void
save(OutputArchive& ar, const arma::Mat<T>& mat, const unsigned int version) {
    size_t size = mat.size();

    ar & mat.n_cols;
    ar & mat.n_rows;

    ar & make_array(mat.memptr(), size);
}

/// @brief Boost deserialization for @a arma::Mat.
template <typename InputArchive, typename T>
void
load(InputArchive& ar, arma::Mat<T>& mat, const unsigned int version) {
    int cols, rows;

    ar & cols;
    ar & rows;

    size_t size = cols * rows;
    T* data = new T[size];

    ar & make_array(data, size);

    mat = arma::Mat<T>(data, rows, cols);
    delete data;
}

/// @brief Boost serialization for @a cv::Mat.
template <typename OutputArchive>
void
save(OutputArchive& ar, const cv::Mat& mat, const unsigned int version) {
    size_t elemSize = mat.elemSize();
    size_t elemType = mat.type();
    size_t dataSize = mat.cols * mat.rows * mat.elemSize();

    ar & mat.cols;
    ar & mat.rows;
    ar & elemSize;
    ar & elemType;

    ar & make_array(mat.ptr(), dataSize);
}

/// @brief Boost deserialization for @a cv::Mat.
template <typename InputArchive>
void
load(InputArchive& ar, cv::Mat& mat, const unsigned int version) {
    int cols, rows;
    size_t elemSize, elemType;

    ar & cols;
    ar & rows;
    ar & elemSize;
    ar & elemType;

    size_t dataSize = cols * rows * elemSize;

    mat.create(rows, cols, elemType);

    ar & make_array(mat.ptr(), dataSize);
}

}} /* namespace boost::serialization */

#endif /* VIS_DETAILS_SERIALIZATION_MATRIX_HPP */

