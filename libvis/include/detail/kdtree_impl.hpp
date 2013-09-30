/**
 * @file detail/kdtree_impl.hpp
 * @brief VLFeat Kd-tree wrapper implementation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_KDTREE_IMPL_HPP
#define VIS_DETAIL_KDTREE_IMPL_HPP

#include "traits.hpp"
#include <boost/assert.hpp>
#include <boost/serialization/array.hpp>
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

void
printTreeInfo(const VlKDForest* forest) {
    for (vl_uindex ti = 0; ti < vl_kdforest_get_num_trees(forest); ++ti) {
        printf("vl_kdforestbuild: tree %llu: depth %llu, num nodes %llu\n",
                ti,
                vl_kdforest_get_depth_of_tree(forest, ti),
                vl_kdforest_get_num_nodes_of_tree(forest, ti));
    }
}

template <typename T>
KDTree<T>::KDTree() {
    forest = NULL;
    dataPtr = NULL;
    dataSize = 0;
}

template <typename T>
KDTree<T>::KDTree(const T* data, vl_size numDimensions, vl_size numSamples,
        vl_size numTrees, bool verbose) {
    dataPtr = NULL;
    dataSize = 0;

    forest = vl_kdforest_new(VlType<T>::type, numDimensions, numTrees, kdtree::distance) ;
    vl_kdforest_set_thresholding_method(forest, kdtree::thresholdingMethod);

    if (verbose) printInfo<T>(forest, numSamples);
    BOOST_ASSERT(numDimensions == vl_kdforest_get_data_dimension(forest));

    vl_kdforest_build(forest, numSamples, data);
    if (verbose) printTreeInfo(forest);
}

template <typename T>
KDTree<T>::KDTree(const cv::Mat& d, vl_size numTrees, bool verbose) {
    BOOST_ASSERT_MSG(d.depth() == cv::DataType<T>::type, "Data is not of type T");
    BOOST_ASSERT_MSG(d.isContinuous(), "Data is not continuous");

    vl_size numDimensions = d.rows;
    vl_size numSamples = d.cols;

    dataSize = numDimensions * numSamples;
    dataPtr = (T*) vl_calloc(sizeof(T), dataSize);

    cv::Mat dt = d.t();
    std::copy(dt.begin<T>(), dt.end<T>(), dataPtr);

    forest = vl_kdforest_new(VlType<T>::type, numDimensions, numTrees, kdtree::distance) ;
    vl_kdforest_set_thresholding_method(forest, kdtree::thresholdingMethod);

    if (verbose) printInfo<T>(forest, numSamples);
    BOOST_ASSERT(numDimensions == vl_kdforest_get_data_dimension(forest));

    vl_kdforest_build(forest, numSamples, dataPtr);
    if (verbose) printTreeInfo(forest);
}

template <typename T>
KDTree<T>::~KDTree() {
    vl_kdforest_delete(forest);
    if (dataPtr != NULL) vl_free(dataPtr);
}

template <typename T>
std::vector<KDTreeNeighbor>
KDTree<T>::search(const T* query, vl_size numQueries, vl_size numNeighbors, vl_size maxNumComparisons) {
    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    BOOST_ASSERT_MSG(numQueries == 1, "Multiple queries not yet supported");

    vl_uint32* indexes = (vl_uint32*) vl_calloc(sizeof(vl_uint32), numNeighbors * numQueries);
    double* distances = (double*) vl_calloc(sizeof(double), numNeighbors * numQueries);

    std::vector<KDTreeNeighbor> results;
    getResults(query, numQueries, numNeighbors, results);

    return results;
}

template <typename T>
std::vector<KDTreeNeighbor>
KDTree<T>::search(const cv::Mat& query, vl_size numNeighbors, vl_size maxNumComparisons) {
    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    BOOST_ASSERT_MSG(query.depth() == cv::DataType<T>::type, "Query is not of type T");
    BOOST_ASSERT_MSG(query.isContinuous(), "Query is not continuous");
    BOOST_ASSERT_MSG(query.cols == 1, "Multiple queries not yet supported");

    vl_size numQueries = query.cols;
    T* queryPtr = (T*) vl_calloc(sizeof(T), vl_kdforest_get_data_dimension(forest));
    std::copy(query.begin<T>(), query.end<T>(), queryPtr);

    std::vector<KDTreeNeighbor> results;
    getResults(queryPtr, numQueries, numNeighbors, results);

    vl_free(queryPtr);
    return results;
}

template <typename T>
void
KDTree<T>::getResults(const T* query, vl_size numQueries, vl_size numNeighbors,
        std::vector<KDTreeNeighbor>& results) {
    vl_uint32* indexes = (vl_uint32*) vl_calloc(sizeof(vl_uint32), numNeighbors * numQueries);
    double* distances = (double*) vl_calloc(sizeof(double), numNeighbors * numQueries);

    vl_kdforest_query_with_array(forest, indexes, numNeighbors, numQueries, distances, query);

    // TODO get results from multiple queries
    for (int i = 0; i < numNeighbors; ++i) {
        KDTreeNeighbor item;
        item.index = indexes[i];
        item.distance = distances[i]; // TODO do not set if not requested

        results.push_back(item);
    }

    vl_free(indexes);
    vl_free(distances);
}

template <typename T>
template <typename Archive>
void
KDTree<T>::save(Archive& ar, const unsigned int version) const {
    ar & forest;

    BOOST_ASSERT_MSG(dataSize > 0, "KDTree has no data");
    ar & dataSize;

    BOOST_ASSERT_MSG(dataPtr != NULL, "dataPtr is NULL");
    ar & boost::serialization::make_array(dataPtr, dataSize);
}

template <typename T>
template <typename Archive>
void
KDTree<T>::load(Archive& ar, const unsigned int version) {
    ar & forest;

    ar & dataSize;
    BOOST_ASSERT_MSG(dataSize > 0, "KDTree has no data");

    dataPtr = new T[dataSize];

    ar & boost::serialization::make_array(dataPtr, dataSize);
    BOOST_ASSERT_MSG(dataPtr != NULL, "dataPtr is NULL");

    forest->data = dataPtr;
}

} /* namespace vis */

#endif /* VIS_DETAIL_KDTREE_IMPL_HPP */

