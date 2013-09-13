/**
 * @file detail/kdtree_impl.hpp
 * @brief VLFeat Kd-tree wrapper implementation
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_KDTREE_IMPL_HPP
#define VIS_DETAIL_KDTREE_IMPL_HPP

#include "traits.hpp"
#include <boost/assert.hpp>

namespace vis {

namespace kdtree {
    static const VlVectorComparisonType distance = VlDistanceL2;
    static const VlKDTreeThresholdingMethod thresholdingMethod = VL_KDTREE_MEDIAN;
    static const vl_size maxNumComparisons = 0;
}

template <typename T>
KDTree<T>::KDTree(const cv::Mat& data, vl_size numTrees) {
    BOOST_ASSERT_MSG(data.depth() == cv::DataType<T>::type, "data is not of type float");

    vl_size numDimensions = data.cols;
    vl_size numSamples = data.rows;
    const T* ptr = data.ptr<T>(0);
    forest = vl_kdforest_new(VlType<T>::type, numDimensions, numTrees, kdtree::distance) ;
    vl_kdforest_set_thresholding_method(forest, kdtree::thresholdingMethod);
    vl_kdforest_build(forest, numSamples, ptr);
}

template <typename T>
KDTree<T>::~KDTree() {
    vl_kdforest_delete(forest);
}

template <typename T>
std::vector<KDTreeNeighbor>
KDTree<T>::search(const cv::Mat& query, vl_size numNeighbors) {
    BOOST_ASSERT_MSG(query.depth() == cv::DataType<T>::type, "query is not of type float");
    const T* ptr = query.ptr<T>(0);

    // TODO make this a method
    vl_kdforest_set_max_num_comparisons(forest, kdtree::maxNumComparisons);

    VlKDForestNeighbor* neighbors = (VlKDForestNeighbor*) vl_malloc(sizeof(VlKDForestNeighbor) * numNeighbors);
    vl_kdforest_query(forest, neighbors, numNeighbors, ptr);

    std::vector<KDTreeNeighbor> results;
    for (int i = 0; i < numNeighbors; ++i) {
        VlKDForestNeighbor* curr = neighbors + i;
        BOOST_ASSERT(curr);
        results.push_back(KDTreeNeighbor(curr));
    }

    vl_free(neighbors); // automatically freed with the forest

    return results;
}

} /* namespace vis */

#endif /* VIS_DETAIL_KDTREE_IMPL_HPP */

