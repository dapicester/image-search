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
#include <boost/foreach.hpp>

namespace perf {

/// Number of times a single test is executed.
static const int NUM_EXECUTIONS = 10;

/// Number of data samples.
static const boost::array<size_t,4> SIZES = { 1e3, 1e4, 1e5, 1e6 };

/// Number of neighbors returned by query.
static const boost::array<size_t,5> NUM_NEIGHBORS = { 1, 5, 10, 20, 50 };

/// Max comparisons for ANN.
static const boost::array<size_t,5> MAX_COMPARISONS = { 0, 10, 100, 1000, 10000 };

/// @brief Build index on given descriptors matrix.
std::vector<timestamp_t>
perfBuildIndex(const cv::Mat& data, vis::DescriptorsType type);

/// @brief Query index with given descriptors matrix.
std::vector<timestamp_t>
perfQueryIndex(const vis::Index& index, const cv::Mat& data, size_t neighbors = 15);

} /* namespace perf */

#endif /* VIS_PERF_INDEX_HPP */

