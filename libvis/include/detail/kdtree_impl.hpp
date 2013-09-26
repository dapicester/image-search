/**
 * @file detail/kdtree_impl.hpp
 * @brief VLFeat Kd-tree wrapper implementation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_KDTREE_IMPL_HPP
#define VIS_DETAIL_KDTREE_IMPL_HPP

#include "traits.hpp"
#include <boost/assert.hpp>
#include <cstdio>

namespace vis {

namespace kdtree {
    static const VlVectorComparisonType distance = VlDistanceL2;
    static const VlKDTreeThresholdingMethod thresholdingMethod = VL_KDTREE_MEDIAN;
}

template <typename T>
void
printInfo(const VlKDForest* forest, vl_size numSamples) {
    printf("vl_kdforestbuild: data %s [%llu x %llu]\n",
            vl_get_type_name(VlType<T>::type),
            vl_kdforest_get_data_dimension(forest),
            numSamples);
    printf("vl_kdforestbuild: threshold selection method: %s\n", "median") ;
    printf("vl_kdforestbuild: number of trees: %llu\n",
            vl_kdforest_get_num_trees(forest));
    printf("vl_kdforestbuild: distance: %s\n",
            vl_get_vector_comparison_type_name(kdtree::distance));
}

template <typename T>
KDTree<T>::KDTree(const T* data, vl_size numDimensions, vl_size numSamples, vl_size numTrees, bool verbose) {
    dataPtr = NULL;
    forest = vl_kdforest_new(VlType<T>::type, numDimensions, numTrees, kdtree::distance) ;
    vl_kdforest_set_thresholding_method(forest, kdtree::thresholdingMethod);

    if (verbose) printInfo<T>(forest, numSamples);
    BOOST_ASSERT(numDimensions == vl_kdforest_get_data_dimension(forest));

    vl_kdforest_build(forest, numSamples, data);
}

template <typename T>
KDTree<T>::KDTree(const cv::Mat& d, vl_size numTrees, bool verbose) {
    BOOST_ASSERT_MSG(d.depth() == cv::DataType<T>::type, "Data is not of type T");
    BOOST_ASSERT_MSG(d.isContinuous(), "Data is not continuous");

    vl_size numDimensions = d.rows;
    vl_size numSamples = d.cols;
    dataPtr = (T*) vl_calloc(sizeof(T), numDimensions * numSamples);
    cv::Mat dt = d.t();
    std::copy(dt.begin<T>(), dt.end<T>(), dataPtr);

    forest = vl_kdforest_new(VlType<T>::type, numDimensions, numTrees, kdtree::distance) ;
    vl_kdforest_set_thresholding_method(forest, kdtree::thresholdingMethod);

    if (verbose) printInfo<T>(forest, numSamples);
    BOOST_ASSERT(numDimensions == vl_kdforest_get_data_dimension(forest));

    vl_kdforest_build(forest, numSamples, dataPtr);
}

template <typename T>
KDTree<T>::~KDTree() {
    vl_kdforest_delete(forest);
    if (dataPtr != NULL) vl_free(dataPtr);
}

template <typename T>
std::vector<KDTreeNeighbor>
KDTree<T>::search(const T* query, vl_size numNeighbors, vl_size maxNumComparisons) {
    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    vl_size numQueries = 1;
    vl_uint32* indexes = (vl_uint32*) vl_calloc(sizeof(vl_uint32), numNeighbors * numQueries);
    double* distances = (double*) vl_calloc(sizeof(double), numNeighbors * numQueries);

    vl_kdforest_query_with_array(forest, indexes, numNeighbors, numQueries, distances, query);

    std::vector<KDTreeNeighbor> results;
    for (int i = 0; i < numNeighbors; ++i) {
        KDTreeNeighbor item;
        item.index = indexes[i];
        item.distance = distances[i]; // TODO do not set if not requested

        results.push_back(item);
    }

    vl_free(indexes);
    vl_free(distances);

    return results;
}

template <typename T>
std::vector<KDTreeNeighbor>
KDTree<T>::search(const cv::Mat& query, vl_size numNeighbors, vl_size maxNumComparisons) {
    BOOST_ASSERT_MSG(query.depth() == cv::DataType<T>::type, "Query is not of type T");
    BOOST_ASSERT_MSG(query.isContinuous(), "Query is not continuous");
    BOOST_ASSERT_MSG(query.cols == 1, "Multiple queries not yet supported");

    //vl_size numQueries = query.cols;
    vl_size numQueries = 1;
    T* queryPtr = (T*) vl_calloc(sizeof(T), vl_kdforest_get_data_dimension(forest));
    std::copy(query.begin<T>(), query.end<T>(), queryPtr);

    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    vl_uint32* indexes = (vl_uint32*) vl_calloc(sizeof(vl_uint32), numNeighbors * numQueries);
    double* distances = (double*) vl_calloc(sizeof(double), numNeighbors * numQueries);

    vl_size numComparisons = vl_kdforest_query_with_array(forest, indexes, numNeighbors, numQueries, distances, queryPtr);

    // TODO get results from multiple queries
    std::vector<KDTreeNeighbor> results;
    for (int i = 0; i < numNeighbors; ++i) {
        KDTreeNeighbor item;
        item.index = indexes[i];
        item.distance = distances[i]; // TODO do not set if not requested

        results.push_back(item);
    }

    vl_free(queryPtr);
    vl_free(indexes);
    vl_free(distances);
    return results;
}

} /* namespace vis */

#endif /* VIS_DETAIL_KDTREE_IMPL_HPP */

