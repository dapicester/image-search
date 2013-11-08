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

static void
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
    forest = nullptr;
    dataPtr = nullptr;
    dataSize = 0;
}

template <typename T>
KDTree<T>::KDTree(const cv::Mat& d, vl_size numTrees) {
    BOOST_ASSERT_MSG(d.depth() == cv::DataType<T>::type, "Data is not of type T");

    vl_size numDimensions = d.rows;
    vl_size numSamples = d.cols;

    dataSize = numDimensions * numSamples;
    dataPtr = (T*) vl_calloc(dataSize, sizeof(T));

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
    if (forest) vl_kdforest_delete(forest);
    if (dataPtr) vl_free(dataPtr);
}

template <typename T>
vl_size
KDTree<T>::getNumTrees() const {
    return vl_kdforest_get_num_trees(forest);
}

// template function declaration
template <typename T, typename N>
void
getResults(VlKDForest* forest, const T* query, vl_size numQueries,
           vl_size numNeighbors, std::vector<N>& results);

// KDTreeNeighbor specialization
template <typename T>
void
getResults(VlKDForest* forest, const T* query, vl_size numQueries,
        vl_size numNeighbors, std::vector<KDTreeNeighbor>& results) {
    vl_uint32* indexes = (vl_uint32*) vl_calloc(numNeighbors * numQueries, sizeof(vl_uint32));
    double* distances = (double*) vl_calloc(numNeighbors * numQueries, sizeof(double));

    vl_kdforest_query_with_array(forest, indexes, numNeighbors, numQueries, distances, query);

    for (int i = 0; i < numNeighbors * numQueries; ++i) {
        KDTreeNeighbor item;
        item.index = indexes[i];
        item.distance = distances[i];

        results.push_back(item);
    }

    vl_free(indexes);
    vl_free(distances);
}

// KDTreeIndex specialization
template <typename T>
void
getResults(VlKDForest* forest, const T* query, vl_size numQueries,
        vl_size numNeighbors, std::vector<KDTreeIndex>& results) {
    vl_uint32* indexes = (vl_uint32*) vl_calloc(numNeighbors * numQueries, sizeof(vl_uint32));

    vl_kdforest_query_with_array(forest, indexes, numNeighbors, numQueries, 0, query);

    for (int i = 0; i < numNeighbors * numQueries; ++i) {
        KDTreeIndex item;
        item.index = indexes[i];

        results.push_back(item);
    }

    vl_free(indexes);
}

template <typename T>
template <typename Record>
std::vector<Record>
KDTree<T>::search(const cv::Mat& query, vl_size numNeighbors, vl_size maxNumComparisons) {
    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    BOOST_ASSERT_MSG(query.depth() == cv::DataType<T>::type, "Query is not of type T");
    BOOST_ASSERT_MSG(query.rows == vl_kdforest_get_data_dimension(forest), "Query has wrong data dimension");

    vl_size numQueries = query.cols;
    T* queryPtr = (T*) vl_calloc(numQueries * query.rows, sizeof(T));

    cv::Mat temp = query.t(); // data is stored by rows, we need columns
    std::copy(temp.begin<T>(), temp.end<T>(), queryPtr);

    std::vector<Record> results;
    getResults(forest, queryPtr, numQueries, numNeighbors, results);

    vl_free(queryPtr);
    return results;
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

    dataPtr = (T*) vl_calloc(dataSize, sizeof(T));

    ar & boost::serialization::make_array(dataPtr, dataSize);
    BOOST_ASSERT_MSG(dataPtr != NULL, "dataPtr is NULL");

    forest->data = dataPtr;
}


} /* namespace vis */

#endif /* VIS_DETAIL_KDTREE_IMPL_HPP */

