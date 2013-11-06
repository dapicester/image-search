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

/// @brief KD-tree search results including index and distance.
struct KDTreeNeighbor {
    vl_uindex index;
    double distance;
};

/// @brief KD-tree search results including only index.
struct KDTreeIndex {
    vl_uindex index;
};

/// @brief KD-tree object.
template <typename T>
class KDTree {
public:
    /// Default constructor.
    KDTree();

    /// Default destructor.
    ~KDTree();

    /// @brief Builds a new KD-Tree retaining a copy of the input matrix.
    KDTree(const cv::Mat& data, vl_size numTrees = 1, bool verbose = false); // TODO add setVerbose(bool)

    /// @return The actual number of trees.
    vl_size getNumTrees() const;

    /// @brief Queries a KD-tree.
    template <typename Record>
    std::vector<Record> search(const cv::Mat& query,
                               vl_size numNeighbors = 1,
                               vl_size maxNumComparisons = 0);

private:

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

#ifdef ENABLE_TESTING
public:
    const VlKDForest* getForest() const { return forest; }
#endif
};

/// Operator << for std::ostream and KDTreeNeighbor.
std::ostream&
operator<<(std::ostream& os, const KDTreeNeighbor& n);

/// Operator << for std::ostream and KDTreeIndex.
std::ostream&
operator<<(std::ostream& os, const KDTreeIndex& i);

} /* namespace vis */

#include "detail/kdtree_impl.hpp"

#endif /* VIS_KDTREE_HPP */

