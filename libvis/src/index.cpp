/**
 * @file index.cpp
 * @brief Visual image search index.
 * @author Paolo D'Apice
 */

#include "index.hpp"
#include "kdtree.hpp"
#include "serialization.hpp"
#include <iostream>

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
    kdtree->build(descriptors.get(), numTrees);
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
    std::cout << "Loading index from file: " << file << " ... ";
    Index* index = vis::load<Index, BinarySerializer>(file);
    std::cout << "done" << std::endl;
    return index;
}

void
Index::save(const fs::path& file) {
    std::cout << "Saving index to file: " << file << " ... ";
    vis::save<Index, BinarySerializer>(file, *this);
    std::cout << "done" << std::endl;
}

} /* namespace vis */

