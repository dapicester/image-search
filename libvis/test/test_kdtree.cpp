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

BOOST_AUTO_TEST_CASE(test_kdtree_mat) {
    cout << "** cv::Mat" << endl;

    int dimension = 2;
    int numData = 100;
    Mat data = getTestData<double>(dimension, numData);
#if PRINT
    cout << data << endl;
#endif
    KDTree<double> tree(data);

    // pick one random record within data
    srand(time(NULL));
    int index = rand() % numData;
    Mat query = data.col(index).clone();
#if PRINT
    cout << "query: " << query << endl;
#endif

    vector<KDTreeNeighbor> results = tree.search(query);
    BOOST_REQUIRE_EQUAL(1, results.size());

    KDTreeNeighbor record = results.front();
#if PRINT
    printf("record: { i=%llu\td=%f }\n", record.index, record.distance);
#endif
    BOOST_REQUIRE_EQUAL(index, record.index);
    BOOST_REQUIRE_EQUAL(0.0, record.distance);
}

