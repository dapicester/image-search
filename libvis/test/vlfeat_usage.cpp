/**
 * @file vlfeat_usage.cpp
 * @brief Usage tests for the VLFeat libraries.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE vlfeat
#include <boost/test/unit_test.hpp>

extern "C" {
#include <vl/random.h>
#include <vl/kdtree.h>
}
#include <random>
#include <cstdlib>

BOOST_AUTO_TEST_CASE(vl_version) {
    VL_PRINT(vl_get_version_string());
    VL_PRINT(vl_configuration_to_string_copy());
}

BOOST_AUTO_TEST_CASE(vl_kdtree) {
    vl_size numDimensions = 2;
    vl_size numSamples    = 100;
    double* data;

    VlKDForest* forest ;

    // Create data
    {
        VlRand* rand = vl_get_rand();
        double* Xi = data = (double*) vl_malloc(sizeof(double) * numDimensions * numSamples);
        for (vl_uindex i = 0; i < numDimensions * numSamples; ++i) {
            *Xi++ = vl_rand_real1(rand);
        }
#if PRINT
        VL_PRINTF("data:\n");
        for (vl_uindex i = 0; i < numDimensions; ++i) {
            VL_PRINTF("[");
            for (vl_uindex j = 0; j < numSamples; ++j) {
                VL_PRINTF("%.6f,", data[i*numDimensions + j]);
            }
            VL_PRINTF("\b]\n");
        }
#endif
    }

     // Build
    {
        VlVectorComparisonType distance = VlDistanceL2;
        vl_type dataType = VL_TYPE_DOUBLE;
        VlKDTreeThresholdingMethod thresholdingMethod = VL_KDTREE_MEDIAN;
        vl_size numTrees = 1;

        forest = vl_kdforest_new(dataType, numDimensions, numTrees, distance);
        vl_kdforest_set_thresholding_method (forest, thresholdingMethod);

        vl_kdforest_build(forest, numSamples, data);

        BOOST_CHECK_EQUAL(numTrees, vl_kdforest_get_num_trees(forest));
#if PRINT
        VL_PRINTF("vl_kdforestbuild: data [%llu x %llu] %s\n",
                  vl_kdforest_get_data_dimension(forest),
                  numSamples,
                  vl_get_type_name(vl_kdforest_get_data_type(forest)));
        VL_PRINTF("vl_kdforestbuild: distance: %s\n",
                  vl_get_vector_comparison_type_name(distance));
        VL_PRINTF("vl_kdforestbuild: number of trees: %llu\n",
                  vl_kdforest_get_num_trees(forest));
        for (vl_uindex i = 0; i < numTrees ; ++i) {
            VL_PRINTF("vl_kdforestbuild: tree %d: data %s [%d x %d]\n",
                i + 1,
                vl_get_type_name (dataType),
                vl_kdforest_get_data_dimension(forest),
                vl_kdforest_get_num_nodes_of_tree(forest, i));
        }
#endif
    }

    // Query
    {
        vl_size numNeighbors = 1;
        vl_size maxNumComparisons = 0;

        vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

        vl_index index = rand() % numSamples;
        double* query = data + index*numDimensions;
#if PRINT
        VL_PRINTF("query: [%.6f, %.6f]\n", query[0], query[1]);
#endif
        VlKDForestNeighbor* neighbors = (VlKDForestNeighbor*) vl_calloc(sizeof(VlKDForestNeighbor), numNeighbors);
        vl_size numVisited = vl_kdforest_query(forest, neighbors, numNeighbors, query);
#if PRINT
        for (vl_uindex i = 0 ; i < numNeighbors; ++ i) {
            VlKDForestNeighbor* curr = neighbors + i;
            VL_PRINTF("record[%d]: { i=%d\td=%f }\n", i, curr->index, curr->distance);
        }
#endif
        BOOST_CHECK_EQUAL(neighbors[0].index, index);
        BOOST_CHECK_EQUAL(neighbors[0].distance, 0.0);

#if PRINT
        VL_PRINT("result: [%f, %f]\n", data[index*numDimensions], data[index*numDimensions + 1]);
#endif
        BOOST_CHECK_EQUAL(query[0], data[index*numDimensions]);
        BOOST_CHECK_EQUAL(query[1], data[index*numDimensions + 1]);

        vl_free(neighbors);
    }

    vl_kdforest_delete(forest);
    vl_free(data);
}

