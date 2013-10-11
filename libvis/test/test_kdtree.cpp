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

using cv::Mat;
using std::vector;
using std::cout;
using std::endl;
using namespace vis;

BOOST_AUTO_TEST_CASE(test_kdtree) {
    int dimension = 2;
    int numData = 100;
    Mat data = getTestData<double>(dimension, numData);

    printmat(data);

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

    printmat(indices);
    printmat(queries);

    {
        // query for index and distance
        vector<KDTreeNeighbor> results = tree.search<KDTreeNeighbor>(queries);
        BOOST_REQUIRE_EQUAL(numQueries, results.size());

        for (int i = 0; i < numQueries; i++) {
            KDTreeNeighbor record = results[i];

            print(record);

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

            print(record);

            BOOST_REQUIRE_EQUAL(indices.at<int>(i), record.index);
        }
    }
}

