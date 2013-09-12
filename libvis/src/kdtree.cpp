#include "kdtree.hpp"
#include <boost/assert.hpp>

using namespace cv;
using std::vector;

namespace vis {

static const VlVectorComparisonType distance = VlDistanceL2;
static const vl_type dataType = VL_TYPE_FLOAT;
static const VlKDTreeThresholdingMethod thresholdingMethod = VL_KDTREE_MEDIAN;
static const vl_size maxNumComparisons = 0;

KDTree::KDTree(const Mat& data, vl_size numTrees) {
    BOOST_ASSERT_MSG(data.depth() == DataType<float>::type, "data is not of type float");

    vl_size numDimensions = data.cols;
    vl_size numSamples = data.rows;
    const float* ptr = data.ptr<float>(0);
    forest = vl_kdforest_new(dataType, numDimensions, numTrees, distance) ;
    vl_kdforest_set_thresholding_method(forest, thresholdingMethod);
    vl_kdforest_build(forest, numSamples, ptr);
}

KDTree::~KDTree() {
    vl_kdforest_delete(forest);
}

vector<KDTreeNeighbor>
KDTree::search(const Mat& query, vl_size numNeighbors) {
    BOOST_ASSERT_MSG(query.depth() == DataType<float>::type, "query is not of type float");
    const float* ptr = query.ptr<float>(0);

    // TODO make this a method
    vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

    VlKDForestNeighbor* neighbors = (VlKDForestNeighbor*) vl_malloc(sizeof(VlKDForestNeighbor) * numNeighbors);
    vl_kdforest_query(forest, neighbors, numNeighbors, ptr);

    vector<KDTreeNeighbor> results;
    for (int i = 0; i < numNeighbors; ++i) {
        VlKDForestNeighbor* curr = neighbors + i;
        BOOST_ASSERT(curr);
        results.push_back(KDTreeNeighbor(curr));
    }

    vl_free(neighbors); // automatically freed with the forest

    return results;
}

} /* namespace vis */

