/**
 * @file index.cpp
 * @brief Visual image search index.
 * @author Paolo D'Apice
 */

#include "vis/index.hpp"
#include "vis/kdtree.hpp"
#include "vis/serialization.hpp"

namespace vis {

namespace fs = boost::filesystem;

Index::Index() : kdtree(new KDTree<float>) {}

Index::~Index() {}

void
Index::build(const std::string& cat, const arma::fmat& data, DescriptorsType t, size_t numTrees) {
    category = cat;
    type = t;
    kdtree->build(data, numTrees);
}

void
Index::build(const std::string& cat, const Descriptors& descriptors, size_t numTrees) {
    category = cat;
    type = descriptors.getType();
    files = descriptors.getFiles();

    kdtree->build(descriptors.data(), numTrees);
}

void
Index::query(const arma::uvec& ids, std::vector<id_type>& results,
        size_t numResults, size_t maxNumComparisons) const {
    std::vector<KDTreeIndex> items = kdtree->search<KDTreeIndex>(ids, numResults, maxNumComparisons);
    for (const KDTreeIndex& item : items) {
        results.push_back(item.index);
    }
}

void
Index::query(const arma::fmat& data, std::vector<id_type>& results,
        size_t numResults, size_t maxNumComparisons) const {
    std::vector<KDTreeIndex> items = kdtree->search<KDTreeIndex>(data, numResults, maxNumComparisons);
    for (const KDTreeIndex& item : items) {
        results.push_back(item.index);
    }
}

Index*
Index::load(const fs::path& file) {
    return vis::load<Index, BinarySerializer>(file);
}

void
Index::save(const fs::path& file) const {
    vis::save<Index, BinarySerializer>(file, *this);
}

} /* namespace vis */

