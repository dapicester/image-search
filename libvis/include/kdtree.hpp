/**
 * @file kdtree.hpp
 * @brief VLFeat KD-tree wrapper
 * @author Paolo D'Apice
 */

#ifndef VIS_KDTREE_HPP
#define VIS_KDTREE_HPP

extern "C" {
#include <vl/kdtree.h>
}
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
#include <opencv2/core/core.hpp>
#include <vector>

namespace vis {

/// @brief KD-tree search results.
struct KDTreeNeighbor {
    vl_uindex index;
    double distance;
};

/// @brief KD-tree object.
template <typename T>
class KDTree {
public:
    KDTree();

    /// @brief Builds a new KD-Tree on the external supplied data.
    KDTree(const T* data, vl_size dimension, vl_size numSamples,
           vl_size numTrees = 1, bool verbose = true);

    /// @brief Builds a new KD-Tree retaining a copy of the input matrix.
    KDTree(const cv::Mat& data, vl_size numTrees = 1, bool verbose = true);

    ~KDTree();

    /// @brief Queries a KD-tree.
    std::vector<KDTreeNeighbor> search(const T* query,
                                       vl_size numQueries,
                                       vl_size numNeighbors = 1,
                                       vl_size maxNumComparisons = 0);

    /// @brief Queries a KD-tree.
    std::vector<KDTreeNeighbor> search(const cv::Mat& query,
                                       vl_size numNeighbors = 1,
                                       vl_size maxNumComparisons = 0);

private:
    void getResults(const T* query, vl_size numQueries, vl_size numNeighbors,
                    std::vector<KDTreeNeighbor>& results);

    friend class boost::serialization::access;

    template <typename Archive>
    void save(Archive& ar, const unsigned int version) const;

    template <typename Archive>
    void load(Archive& ar, const unsigned int version);

    BOOST_SERIALIZATION_SPLIT_MEMBER();
private:
    VlKDForest* forest;
    T* dataPtr;
    vl_size dataSize;
};

} /* namespace vis */

#include "detail/kdtree_impl.hpp"

#endif /* VIS_KDTREE_HPP */

