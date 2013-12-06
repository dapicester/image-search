/**
 * @file test_kdtree.cpp
 * @brief Kd-tree test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kdtree
#include <boost/test/unit_test.hpp>

#include "utils/data.hpp"
#include "utils/print.hpp"

#include "vis/kdtree.hpp"

#include <iostream>
#include <random>

using vis::KDTree;
using vis::KDTreeNeighbor;
using vis::KDTreeIndex;

BOOST_AUTO_TEST_CASE(test_kdtree) {
    int dimension = 2;
    int numData = 100;
    arma::mat data = test::testData<double>(dimension, numData);

    printmat(data);

    KDTree<double> tree;
    tree.build(data);

    int numQueries = 2;
    arma::uvec indices(numQueries);
    arma::mat queries(dimension, numQueries);

    // pick random records within data
    for (int i = 0; i < numQueries; i++) {
        int index = rand() % numData;
        indices(i) = index;
        queries.col(i) = data.col(index);
    }

    printmat(indices);
    printmat(queries);

    {
        // query for index and distance
        std::vector<KDTreeNeighbor> results = tree.search<KDTreeNeighbor>(queries);
        BOOST_REQUIRE_EQUAL(numQueries, results.size());

        for (int i = 0; i < numQueries; i++) {
            KDTreeNeighbor record = results[i];

            printvar(record);

            BOOST_REQUIRE_EQUAL(indices(i), record.index);
            BOOST_REQUIRE_EQUAL(0.0, record.distance);
        }
    }

    {
        // query for index
        std::vector<KDTreeIndex> results = tree.search<KDTreeIndex>(queries);
        BOOST_REQUIRE_EQUAL(numQueries, results.size());

        for (int i = 0; i < numQueries; i++) {
            KDTreeIndex record = results[i];

            printvar(record);

            BOOST_REQUIRE_EQUAL(indices.at(i), record.index);
        }
    }

    {
        // query using indices
        std::vector<KDTreeIndex> results = tree.search<KDTreeIndex>(indices);
        BOOST_REQUIRE_EQUAL(numQueries, results.size());

        for (int i = 0; i < numQueries; i++) {
            KDTreeIndex record = results[i];

            printvar(record);

            BOOST_REQUIRE_EQUAL(indices.at(i), record.index);
        }
    }
}

