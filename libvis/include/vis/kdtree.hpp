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
#include <armadillo>
#include <boost/serialization/access.hpp>
#include <boost/serialization/split_member.hpp>
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
    void build(const arma::Mat<T>& data, unsigned numTrees = 1);

    /// @return The actual number of trees.
    vl_size getNumTrees() const;

    /// @brief Queries a KD-tree.
    template <typename Record>
    std::vector<Record> search(const arma::uvec& indices,
                               unsigned numNeighbors = 1,
                               unsigned maxNumComparisons = 0);

    /// @brief Queries a KD-tree.
    template <typename Record>
    std::vector<Record> search(const arma::Mat<T>& query,
                               unsigned numNeighbors = 1,
                               unsigned maxNumComparisons = 0);

    /// Set verbosity.
    void setVerbose(bool val) { verbose = val; }

private:

    friend class boost::serialization::access;

    template <typename Archive>
    void save(Archive& ar, const unsigned int version) const;

    template <typename Archive>
    void load(Archive& ar, const unsigned int version);

    BOOST_SERIALIZATION_SPLIT_MEMBER();

private:
    bool verbose = false;
    VlKDForest* forest = nullptr;
    arma::Mat<T> data;

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

