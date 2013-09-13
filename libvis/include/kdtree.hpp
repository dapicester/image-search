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
    KDTreeNeighbor(VlKDForestNeighbor* n)
        : index(n->index), distance(n->distance) {}

    vl_uindex index;
    double distance;
};

template <typename T>
struct KDTree {
    KDTree(const cv::Mat& data, vl_size numTrees = 1);
    ~KDTree();

    std::vector<KDTreeNeighbor> search(const cv::Mat& query, vl_size numNeighbors = 1);

private:
    VlKDForest* forest ;
};

} /* namespace vis */

#include "detail/kdtree_impl.hpp"

#endif /* VIS_KDTREE_HPP */

