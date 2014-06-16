/**
 * @file detail/kmeans_impl.hpp
 * @brief VLFeat K-means wrapper implementation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_KMEANS_IMPL_HPP
#define VIS_DETAIL_KMEANS_IMPL_HPP

#include "../traits.hpp"

#include <boost/assert.hpp>
#include <cstdio>

namespace vis {

namespace kmeans {
    static const VlVectorComparisonType distance = VlDistanceL2;
    static const VlKMeansAlgorithm algorithm = VlKMeansElkan;
    static const VlKMeansInitialization initialization = VlKMeansPlusPlus;
    static const vl_size maxNumIterations = 100;
    static const vl_size maxNumComparisons = 100;
    static const vl_size numRepetitions = 1;
    static const vl_size numTrees = 3;
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
    vl_kmeans_set_verbosity(kmeans, verbose);
}

template <typename T>
KMeans<T>::~KMeans() {
    vl_kmeans_delete(kmeans);
}

static void
printInfo(const VlKMeans* kmeans, vl_size dimension, vl_size numData, vl_size numCenters) {
    printf("kmeans: Initialization = plusplus\n");
    printf("kmeans: Algorithm = Elkan\n");
    printf("kmeans: MaxNumIterations = %llu\n", vl_kmeans_get_max_num_iterations(kmeans));
    printf("kmeans: NumRepetitions = %llu\n", vl_kmeans_get_num_repetitions(kmeans));
    printf("kmeans: data type = %s\n", vl_get_type_name(vl_kmeans_get_data_type(kmeans)));
    printf("kmeans: distance = %s\n", vl_get_vector_comparison_type_name(vl_kmeans_get_distance(kmeans)));
    printf("kmeans: data dimension = %llu\n", dimension);
    printf("kmeans: num. data points = %llu\n", numData) ;
    printf("kmeans: num. centers = %llu\n", numCenters) ;
    printf("kmeans: max num. comparisons = %llu\n", vl_kmeans_get_max_num_comparisons(kmeans));
    printf("kmeans: num. trees = %llu\n", vl_kmeans_get_num_trees(kmeans));
    printf("\n") ;
}

template <typename T>
arma::Mat<T>
KMeans<T>::cluster(const arma::Mat<T>& data, unsigned numCenters) {
    vl_size dimension = data.n_rows;
    vl_size numData = data.n_cols;
    const T* ptr = data.memptr();

    if (verbose) printInfo(kmeans, dimension, numData, numCenters);

    vl_kmeans_cluster(kmeans, ptr, dimension, numData, numCenters) ;
    T* centers = (T*) vl_kmeans_get_centers(kmeans);

    BOOST_ASSERT(numCenters == vl_kmeans_get_num_centers(kmeans));
    BOOST_ASSERT(dimension == vl_kmeans_get_dimension(kmeans));

    return arma::Mat<T>(centers, dimension, numCenters);
}

} /* namespace vis */

#endif /* VIS_DETAIL_KMEANS_IMPL_HPP */

