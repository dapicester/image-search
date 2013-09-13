/**
 * @file kmeans.hpp
 * @brief VLFeat K-means wrapper
 * @author Paolo D'Apice
 */

#ifndef VIS_KMEANS_HPP
#define VIS_KMEANS_HPP

#include "traits.hpp"
extern "C" {
#include <vl/kmeans.h>
}
#include <opencv2/core/core.hpp>

namespace vis {

// defaults
static const VlVectorComparisonType distance = VlDistanceL2 ;
static const int verbosity = 1;
static const vl_size maxiter = 10;
static const vl_size maxComp = 100;
static const vl_size maxrep = 1;
static const vl_size ntrees = 3;
static const VlKMeansAlgorithm algorithm = VlKMeansElkan;

// TODO wrap into a class

/// @brief Compute K-means.
template <typename T>
VlKMeans* kmeans(const cv::Mat& data, size_t numCenters) {
    vl_size numDimensions = data.rows;
    vl_size numData = data.cols;
    const T* ptr = data.ptr<T>(0);

    VlKMeans* kmeans = vl_kmeans_new(VlType<T>::type, distance);

    vl_kmeans_set_verbosity(kmeans, verbosity);
    vl_kmeans_set_max_num_iterations(kmeans, maxiter);
    vl_kmeans_set_max_num_comparisons(kmeans, maxComp);
    vl_kmeans_set_num_repetitions(kmeans, maxrep);
    vl_kmeans_set_num_trees(kmeans, ntrees);
    vl_kmeans_set_algorithm(kmeans, algorithm);

    vl_kmeans_cluster(kmeans, ptr, numDimensions, numData, numCenters);

    return kmeans;
}

} /* namespace vis */

#endif /* VIS_KMEANS_HPP */

