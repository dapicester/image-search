/**
 * @file kdtree.hpp
 * @brief VLFeat Kd-tree wrapper
 * @author Paolo D'Apice
 */

#ifndef VIS_KDTREE_HPP
#define VIS_KDTREE_HPP

extern "C" {
#include <vl/kdtree.h>
}
#include <opencv2/core/core.hpp>
#include <vector>

namespace vis {

struct KDTreeNeighbor {
    vl_uindex index;
    double distance;
};

template <typename T>
class KDTree {
public:
    /// Build a new KD-Tree on the external supplied data.
    KDTree(const T* data, vl_size dimension, vl_size numSamples,
           vl_size numTrees = 1, bool verbose = true);

    /// Build a new KD-Tree retaining a copy of the input matrix.
    KDTree(const cv::Mat& data, vl_size numTrees = 1, bool verbose = true);

    ~KDTree();

    std::vector<KDTreeNeighbor> search(const T* query,
                                       vl_size numNeighbors = 1,
                                       vl_size maxNumComparisons = 0);

    std::vector<KDTreeNeighbor> search(const cv::Mat& query,
                                       vl_size numNeighbors = 1,
                                       vl_size maxNumComparisons = 0);

private:
    void getResults(const T* query, vl_size numQueries, vl_size numNeighbors,
                    std::vector<KDTreeNeighbor>& results);

private:
    VlKDForest* forest;
    T* dataPtr;
};

} /* namespace vis */

#include "detail/kdtree_impl.hpp"

#endif /* VIS_KDTREE_HPP */

