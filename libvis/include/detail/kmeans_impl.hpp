/**
 * @file detail/kmeans_impl.hpp
 * @brief VLFeat K-means wrapper implementation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_KMEANS_IMPL_HPP
#define VIS_DETAIL_KMEANS_IMPL_HPP

#include "traits.hpp"
#include <boost/assert.hpp>

namespace vis {

namespace kmeans {
    static const VlVectorComparisonType distance = VlDistanceL2 ;
    static const VlKMeansAlgorithm algorithm = VlKMeansElkan;
    static const VlKMeansInitialization initialization = VlKMeansPlusPlus;
    static const vl_size maxNumIterations = 100;
    static const vl_size maxNumComparisons = 100;
    static const vl_size numRepetitions = 1;
    static const vl_size numTrees = 3;
    static const int verbosity = 1;
}

template <typename T>
KMeans<T>::KMeans() {
    kmeans = vl_kmeans_new(VlType<T>::type, kmeans::distance);
    vl_kmeans_set_algorithm(kmeans, kmeans::algorithm);
    vl_kmeans_set_initialization (kmeans, kmeans::initialization) ;
    vl_kmeans_set_max_num_iterations(kmeans, kmeans::maxNumIterations);
    vl_kmeans_set_max_num_comparisons(kmeans, kmeans::maxNumComparisons);
    vl_kmeans_set_num_repetitions(kmeans, kmeans::numRepetitions);
    vl_kmeans_set_num_trees(kmeans, kmeans::numTrees);
    vl_kmeans_set_verbosity(kmeans, kmeans::verbosity);
}

template <typename T>
KMeans<T>::~KMeans() {
    vl_kmeans_delete(kmeans);
}

template <typename T>
cv::Mat
KMeans<T>::cluster(const cv::Mat& data, vl_size numCenters) {
    vl_size dimension = data.rows;
    vl_size numData = data.cols;
    const T* ptr = data.ptr<T>(0);

    vl_kmeans_cluster(kmeans, ptr, dimension, numData, numCenters) ;
    T* centers = (T*) vl_kmeans_get_centers(kmeans);
    BOOST_ASSERT(numCenters == vl_kmeans_get_num_centers(kmeans));

    return cv::Mat(dimension, numCenters, cv::DataType<T>::type, centers).clone();
}

} /* namespace vis */

#endif /* VIS_DETAIL_KMEANS_IMPL_HPP */

