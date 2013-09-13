/**
 * @file test_kdtree.cpp
 * @brief Kd-tree test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kdtree
#include <boost/test/unit_test.hpp>

#include "kdtree.hpp"
#include "test_utils.hpp"
#include <iostream>
#include <cstdlib>
#include <cstdio>

using cv::Mat;
using std::vector;
using std::cout;
using std::endl;
using namespace vis;

#define PRINT 1

BOOST_AUTO_TEST_CASE(test_kdtree_ptr) {
    cout << "** pointer" << endl;

    int dimension = 2;
    int numData = 100;
    double* data = getTestDataPtr<double>(dimension, numData);
#if PRINT
    printf("[");
    for (int i = 0; i < dimension * numData; i += 2) {
        printf("%f ", data[i]);
    }
    printf("\n ");
    for (int i = 1; i < dimension * numData; i += 2) {
        printf("%f ", data[i]);
    }
    printf("\b]\n");
#endif
    KDTree<double> tree(data, dimension, numData);

    // pick one random record within data
    srand(time(NULL));
    int index = rand() % numData;
    double* query = data + index * dimension;
#if PRINT
    printf("query: [%f, %f]\n", query[0], query[1]);
#endif

    vector<KDTreeNeighbor> results = tree.search(query);
    BOOST_REQUIRE_EQUAL(1, results.size());

    KDTreeNeighbor record = results.front();
#if PRINT
    printf("record: { i=%llu\td=%f }\n", record.index, record.distance);
#endif
    BOOST_REQUIRE_EQUAL(index, record.index);
    BOOST_REQUIRE_EQUAL(0.0, record.distance);

    delete[] data;
}

BOOST_AUTO_TEST_CASE(test_kdtree_vec) {
    cout << "** std::vector" << endl;

    int dimension = 2;
    int numData = 100;
    vector<double> data = getTestDataVec<double>(dimension, numData);
#if PRINT
    cout << "[";
    for (int i = 0; i < dimension * numData; i += 2)
        cout << data[i] << " ";
    cout << endl << " ";
    for (int i = 1; i < dimension * numData; i += 2)
        cout << data[i] << " ";
    cout << "\b]" << endl;
#endif
    KDTree<double> tree(&data[0], dimension, numData);

    // pick one random record within data
    srand(time(NULL));
    int index = rand() % numData;
    vector<double> query(&data[index*dimension], &data[index*dimension + dimension]);
#if PRINT
    cout << "query: " << query << endl;
#endif

    vector<KDTreeNeighbor> results = tree.search(&query[0]);
    BOOST_REQUIRE_EQUAL(1, results.size());

    KDTreeNeighbor record = results.front();
#if PRINT
    printf("record: { i=%llu\td=%f }\n", record.index, record.distance);
#endif
    BOOST_REQUIRE_EQUAL(index, record.index);
    BOOST_REQUIRE_EQUAL(0.0, record.distance);
}

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

