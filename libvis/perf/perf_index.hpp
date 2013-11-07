/**
 * @file perf_index.hpp
 * @brief Performance tests on index.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_INDEX_HPP
#define VIS_PERF_INDEX_HPP

#include "descriptors_type.hpp"
#include "index.hpp"
#include "utils/timer.hpp"
#include <boost/array.hpp>

namespace perf {

/// Number of times a single test is executed.
static const int NUM_EXECUTIONS = 10;

/// Number of data samples.
static const std::array<size_t,4> NUM_DATA = { 1000, 10000, 100000, 1000000 };

/// Number of trees in the kd-forest.
static const std::array<size_t,4> NUM_TREES = { 1, 2, 4, 8 };

/// Number of neighbors returned by query.
static const std::array<size_t,5> NUM_NEIGHBORS = { 1, 5, 10, 20, 50 };

/// Max comparisons for ANN.
static const std::array<size_t,5> MAX_COMPARISONS = { 0, 1000000, 100000, 10000, 100 };

/// @brief Build index on given descriptors matrix.
std::vector<Timer::timestamp_t>
buildIndex(const cv::Mat& data, vis::DescriptorsType type, size_t numTrees = 1);

/// @brief Query index with given descriptors matrix.
std::vector<Timer::timestamp_t>
queryIndex(const vis::Index& index, const cv::Mat& data,
               size_t neighbors = 15, size_t maxComparisons = 0);

} /* namespace perf */

#endif /* VIS_PERF_INDEX_HPP */

