/**
 * @file perf_index.hpp
 * @brief Performance tests on index.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_INDEX_HPP
#define VIS_PERF_INDEX_HPP

#include "descriptors_type.hpp"
#include "index.hpp"
#include "utils/posixtimer.hpp"
#include <boost/array.hpp>

namespace perf {

/// Number of times a single test is executed.
static const int NUM_EXECUTIONS = 10;
/// Data numbers.
static const boost::array<size_t,3> SIZES = { 1e4, 1e5, 1e6 };

//static const boost::array<vis::DescriptorsType,3> TYPES = { vis::HSV, vis::HOG, vis::HOG_HSV };

/// @brief Build index on given descriptors matrix.
std::vector<timestamp_t>
perfBuildIndex(const cv::Mat& data, vis::DescriptorsType type);

/// @brief Query index with given descriptors matrix.
std::vector<timestamp_t>
perfQueryIndex(const vis::Index& index, const cv::Mat& data, size_t neighbors = 15);

} /* namespace perf */

#endif /* VIS_PERF_INDEX_HPP */

