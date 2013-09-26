/**
 * @file serialization.hpp
 * @brief Data serialization
 * @author Paolo D'Apice
 */

#ifndef VIS_SERIALIZATION_HPP
#define VIS_SERIALIZATION_HPP

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/array.hpp>
#include <opencv2/core/core.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(cv::Mat)

namespace boost { namespace serialization {

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

    ar & boost::serialization::make_array(mat.ptr(), dataSize);
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

    ar & boost::serialization::make_array(mat.ptr(), dataSize);
}

}} /* namespace boost::serialization */

#endif /* VIS_SERIALIZATION_HPP */

