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
KDTree<T>::KDTree() {}

template <typename T>
KDTree<T>::~KDTree() {
    if (forest) vl_kdforest_delete(forest);
}

template <typename T>
void
KDTree<T>::build(const arma::Mat<T>& d, unsigned numTrees) {
    data = d;

    vl_size numDimensions = d.n_rows;
    vl_size numSamples = d.n_cols;
    vl_size dataSize = numDimensions * numSamples;

    forest = vl_kdforest_new(VlType<T>::type, numDimensions, numTrees, kdtree::distance) ;
    vl_kdforest_set_thresholding_method(forest, kdtree::thresholdingMethod);

    if (verbose) printInfo<T>(forest, numSamples);
    BOOST_ASSERT(numDimensions == vl_kdforest_get_data_dimension(forest));

    vl_kdforest_build(forest, numSamples, data.memptr());
    if (verbose) printTreeInfo(forest);
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
KDTree<T>::search(const arma::Mat<T>& query,
        unsigned numNeighbors, unsigned maxNumComparisons) {
    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    BOOST_ASSERT_MSG(query.n_rows == vl_kdforest_get_data_dimension(forest),
            "Query has wrong data dimension");

    vl_size numQueries = query.n_cols;

    std::vector<Record> results;
    getResults(forest, query.memptr(), numQueries, numNeighbors, results);

    return results;
}

template <typename T>
template <typename Archive>
void
KDTree<T>::save(Archive& ar, const unsigned int version) const {
    ar & forest;
    ar & data;
}

template <typename T>
template <typename Archive>
void
KDTree<T>::load(Archive& ar, const unsigned int version) {
    ar & forest;
    ar & data;

    forest->data = data.memptr();
}

inline
std::ostream&
operator<<(std::ostream& os, const KDTreeNeighbor& n) {
    os << "{" << n.index << "," << n.distance << "}";
    return os;
}

inline
std::ostream&
operator<<(std::ostream& os, const KDTreeIndex& i) {
    os << "{" << i.index << "}";
    return os;
}

} /* namespace vis */

#endif /* VIS_DETAIL_KDTREE_IMPL_HPP */

