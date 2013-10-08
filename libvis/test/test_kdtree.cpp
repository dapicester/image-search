/**
 * @file test_kdtree.cpp
 * @brief Kd-tree test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kdtree
#include <boost/test/unit_test.hpp>

#include "kdtree.hpp"
#include "utils/data.hpp"
#include "utils/print.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>

using cv::Mat;
using std::vector;
using std::cout;
using std::endl;
using namespace vis;

#define PRINT 1

BOOST_AUTO_TEST_CASE(test_kdtree) {
    int dimension = 2;
    int numData = 100;
    Mat data = getTestData<double>(dimension, numData);
#if PRINT
    cout << "data:\n" << data << endl;
#endif
    KDTree<double> tree(data);

    int numQueries = 2;
    Mat indices(1, numQueries, cv::DataType<int>::type);
    Mat queries(dimension, numQueries, data.type());

    // pick random records within data
    srand(time(NULL));
    for (int i = 0; i < numQueries; i++) {
        int index = rand() % numData;
        indices.at<int>(i) = index;
        data.col(index).copyTo(queries.col(i));
    }
#if PRINT
    cout << "indices: " << indices << endl;
    cout << "queries:\n" << queries << endl;
#endif

    {
        // query for index and distance
        vector<KDTreeNeighbor> results = tree.search<KDTreeNeighbor>(queries);
        BOOST_REQUIRE_EQUAL(numQueries, results.size());

        for (int i = 0; i < numQueries; i++) {
            KDTreeNeighbor record = results[i];
#if PRINT
            printf("record[%d]: { i=%llu\td=%f }\n", i, record.index, record.distance);
#endif
            BOOST_REQUIRE_EQUAL(indices.at<int>(i), record.index);
            BOOST_REQUIRE_EQUAL(0.0, record.distance);
        }
    }

    {
        // query for index
        vector<KDTreeIndex> results = tree.search<KDTreeIndex>(queries);
        BOOST_REQUIRE_EQUAL(numQueries, results.size());

        for (int i = 0; i < numQueries; i++) {
            KDTreeIndex record = results[i];
#if PRINT
            printf("record[%d]: { i=%llu }\n", i, record.index);
#endif
            BOOST_REQUIRE_EQUAL(indices.at<int>(i), record.index);
        }
    }
}

