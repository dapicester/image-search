/**
 * @file index.cpp
 * @brief Visual image search index.
 * @author Paolo D'Apice
 */

#include "index.hpp"
#include "kdtree.hpp"
#include "serialization.hpp"
#include <boost/foreach.hpp>
#include <cstdio>

namespace vis {

namespace fs = boost::filesystem;

Index::Index() {}

Index::~Index() {}

void
Index::build(const std::string& cat, const cv::Mat& data, DescriptorsType t) {
    category = cat;
    type = t;
    kdtree.reset(new KDTree<float>(data));
}

void
Index::build(const std::string& cat, const Descriptors& descriptors) {
    category = cat;
    type = descriptors.getType();
    kdtree.reset(new KDTree<float>(descriptors.get()));
}

void
Index::query(const cv::Mat& data, std::vector<id_type>& results, size_t numResults) {
    std::vector<KDTreeIndex> items = kdtree->search<KDTreeIndex>(data, numResults);
    BOOST_FOREACH(const KDTreeIndex& item, items) {
        results.push_back(item.index);
    }
}

Index*
Index::load(const fs::path& file) {
    printf("Loading index from file %s ... ", file.string().c_str());
    Index* index = vis::load<Index, BinarySerializer>(file);
    printf("done\n");
    return index;
}

void
Index::save(const fs::path& file) {
    printf("Saving index to file %s ...", file.string().c_str());
    vis::save<Index, BinarySerializer>(file, *this);
    printf("done\n");
}

} /* namespace vis */

