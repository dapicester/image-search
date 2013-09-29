/**
 * @file utils/serialization_kdtree.hpp
 * @brief VLFeat KD-tree serialization
 * @author Paolo D'Apice
 * @author Adrea Vedaldi
 */

#ifndef VIS_DETAIL_SERIALIZATION_KDTREE_HPP
#define VIS_DETAIL_SERIALIZATION_KDTREE_HPP

#include <boost/assert.hpp>
#include <boost/serialization/split_free.hpp>
extern "C" {
#include <vl/kdtree.h>
}

#include <iostream>

BOOST_SERIALIZATION_SPLIT_FREE(VlKDForest)

/// Taken from VLFeat's toolbox/misc/kdtree.h
void
restore_parent_recursively(VlKDTree* tree, int nodeIndex, int* numNodesToVisit) {
    VlKDTreeNode* node = tree->nodes + nodeIndex;
    int lowerChild = node->lowerChild;
    int upperChild = node->upperChild;

    // TODO always enable this check
    BOOST_ASSERT_MSG(*numNodesToVisit != 0, "forest.trees has an inconsistent tree structure.");

    *numNodesToVisit -= 1;

    if (lowerChild >= 0) {
        VlKDTreeNode* child = tree->nodes + lowerChild;
        child->parent = nodeIndex;
        restore_parent_recursively(tree, lowerChild, numNodesToVisit);
    }

    if (upperChild >= 0) {
    VlKDTreeNode* child = tree->nodes + upperChild;
        child->parent = nodeIndex;
        restore_parent_recursively (tree, upperChild, numNodesToVisit);
    }
}

namespace boost { namespace serialization {

template <typename Archive>
void serialize(Archive& ar, VlKDTreeNode& n, const unsigned int version) {
}

#if 1
#  define log(X) std::cout << #X << ": " << X << std::endl;
#else
#  define log(x);
#endif

/// @brief Boost serialization for @a VlKDForest
template <typename OutputArchive>
void
save(OutputArchive& ar, const VlKDForest& f, const unsigned int version) {
    ar & f.distance;
    ar & f.thresholdingMethod;
    ar & f.dimension;
    ar & f.numData;
    ar & f.dataType;
    ar & f.numTrees;

    for (vl_uindex ti = 0; ti < f.numTrees; ++ti) {
        const VlKDTree* tree = f.trees[ti];

        ar & tree->numUsedNodes;
        for (vl_uindex ni = 0; ni < tree->numUsedNodes; ++ni) {
            const VlKDTreeNode* node = tree->nodes + ni;
            ar & node->lowerChild;
            ar & node->upperChild;
            ar & node->splitDimension;
            ar & node->splitThreshold;
            ar & node->lowerBound;
            ar & node->upperBound;
        }

        for (vl_uindex di = 0; di < f.numData; ++di) {
            ar & tree->dataIndex[di].index;
        }
    }
}

/// @brief Boost deserialization for @a VlKDForest
template <typename InputArchive>
void
load(InputArchive& ar, VlKDForest& forest, const unsigned int version) {
    VlVectorComparisonType distance;
    ar & distance;
    BOOST_ASSERT_MSG(distance == VlDistanceL2
                     or distance == VlDistanceL1,
                     "forest.distance is neither L2 nor L1");

    VlKDTreeThresholdingMethod thresholdingMethod;
    ar & thresholdingMethod;
    BOOST_ASSERT_MSG(thresholdingMethod == VL_KDTREE_MEDIAN
                     or thresholdingMethod == VL_KDTREE_MEAN,
                     "forest.thresholdMethod is neither MEAN nor MEDIAN");

    vl_size dimension;
    ar & dimension;
    BOOST_ASSERT_MSG(dimension > 0,
                     "forest.dimension is not positive");

    vl_size numData;
    ar & numData;
    BOOST_ASSERT_MSG(numData > 0,
                     "forest.numData is not positive");

    vl_type dataType;
    ar & dataType;
    BOOST_ASSERT_MSG(dataType == VL_TYPE_FLOAT or dataType == VL_TYPE_DOUBLE,
                     "forest.dataType is neither FLOAT nor DOUBLE");

    vl_size numTrees;
    ar & numTrees;
    BOOST_ASSERT_MSG(numTrees >= 1, "forest.numTrees is not greater than or equal to 1");

    VlKDForest* f = vl_kdforest_new(dataType, dimension, numTrees, distance);
    f->numData = numData;
    f->trees = (VlKDTree**) vl_malloc(sizeof(VlKDTree*) * numTrees);

    vl_size maxNumNodes = 0;

    for (vl_uindex ti = 0; ti < numTrees; ++ti) {
        VlKDTree* tree = (VlKDTree*) vl_malloc(sizeof(VlKDTree));

        vl_size numUsedNodes;
        ar & numUsedNodes;

        maxNumNodes += numUsedNodes;

        tree->numAllocatedNodes = numUsedNodes;
        tree->numUsedNodes = numUsedNodes;

        tree->nodes = (VlKDTreeNode*) vl_malloc(sizeof(VlKDTreeNode) * numUsedNodes);
        tree->dataIndex = (VlKDTreeDataIndexEntry*) vl_malloc(sizeof(VlKDTreeDataIndexEntry) * numData);

        for (vl_uindex ni = 0; ni < numUsedNodes; ++ni) {
            VlKDTreeNode* node = tree->nodes + ni;
            ar & node->lowerChild;
            ar & node->upperChild;
            ar & node->splitDimension;
            ar & node->splitThreshold;
            ar & node->lowerBound;
            ar & node->upperBound;
        }

        for (vl_uindex di = 0; di < numData; ++di) {
            ar & tree->dataIndex[di].index;
        }

        int numNodesToVisit = tree->numUsedNodes;
        restore_parent_recursively(tree, 0, &numNodesToVisit);

        f->trees[ti] = tree;
    }

    f->maxNumNodes = maxNumNodes;
    f->data = NULL;

    forest = *f;
}

}} /* namespace boost::serialization */

#endif /* VIS_DETAIL_SERIALIZATION_KDTREE_HPP */

