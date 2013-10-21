/**
 * @file index.cpp
 * @brief Visual image search index.
 * @author Paolo D'Apice
 */

#include "index.hpp"
#include "kdtree.hpp"
#include <boost/foreach.hpp>

namespace vis {

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

} /* namespace vis */

