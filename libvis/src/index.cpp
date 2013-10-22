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
Index::build(const std::string& cat, const cv::Mat& data, IndexType t) {
    category = cat;
    type = t;
    kdtree.reset(new KDTree<float>(data));
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
    BinarySerializer<Index>::Loader loader;
    Index* index = new Index;
    loader(file.string().c_str(), *index);
    printf("done\n");
    return index;
}

void
Index::save(const fs::path& file) {
    printf("Saving index to file %s ...", file.string().c_str());
    BinarySerializer<Index>::Saver saver;
    saver(file.string().c_str(), *this);
    printf("done\n");
}

} /* namespace vis */

