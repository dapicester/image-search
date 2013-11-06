/**
 * @file   test_vl_kdtree.c
 * @brief  Test VLFeat's kd-tree
 * @author Paolo D'Apice
 **/

#include <vl/random.h>
#include <vl/kdtree.h>
#include <stdlib.h>

#define PRINT 0

int main(int argc, char** argv) {
    vl_size numDimensions = 2;
    vl_size numSamples    = 100;
    double* data ;

    VlKDForest* forest ;

    unsigned int i, j;

    // Create data
    {
        VlRand* rand = vl_get_rand();
        vl_uindex i;
        double* Xi = data = vl_malloc(sizeof(double) * numDimensions * numSamples);
        for (i = 0; i < numDimensions * numSamples; ++i) {
            *Xi++ = vl_rand_real1(rand);
        }
#if PRINT
        VL_PRINTF("data:\n");
        for (i = 0; i < numDimensions; ++i) {
            VL_PRINTF("[");
            for (j = 0; j < numSamples; ++j) {
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

        assert(numTrees == vl_kdforest_get_num_trees(forest));
#if PRINT
        VL_PRINTF("vl_kdforestbuild: data [%llu x %llu] %s\n",
                  vl_kdforest_get_data_dimension(forest),
                  numSamples,
                  vl_get_type_name(vl_kdforest_get_data_type(forest)));
        VL_PRINTF("vl_kdforestbuild: distance: %s\n",
                  vl_get_vector_comparison_type_name(distance));
        VL_PRINTF("vl_kdforestbuild: number of trees: %llu\n",
                  vl_kdforest_get_num_trees(forest));
        for (i = 0; i < numTrees ; ++i ) {
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
        vl_index index;
        double* query;
        vl_size numVisited;

        vl_kdforest_set_max_num_comparisons(forest, maxNumComparisons);

        srand(time(NULL));
        index = rand() % numSamples;
        query = data + index*numDimensions;
#if PRINT
        VL_PRINTF("query: [%.6f, %.6f]\n", query[0], query[1]);
#endif
        VlKDForestNeighbor* neighbors = vl_calloc(sizeof(VlKDForestNeighbor), numNeighbors);
        numVisited = vl_kdforest_query(forest, neighbors, numNeighbors, query);
#if PRINT
        for (i = 0 ; i < numNeighbors; ++ i) {
            VlKDForestNeighbor* curr = neighbors + i;
            VL_PRINTF("record[%d]: { i=%d\td=%f }\n", i, curr->index, curr->distance);
        }
#endif
        assert(neighbors[0].index == index);
        assert(neighbors[0].distance == 0.0);

#if PRINT
        VL_PRINT("result: [%f, %f]\n", data[index*numDimensions], data[index*numDimensions + 1]);
#endif
        assert(query[0] == data[index*numDimensions]);
        assert(query[1] == data[index*numDimensions + 1]);

        vl_free(neighbors);
    }

    vl_kdforest_delete(forest);
    vl_free(data);

    return 0 ;
}

