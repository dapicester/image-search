/**
 * @file test_kmeans.cpp
 * @brief K-means test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kmeans
#include <boost/test/unit_test.hpp>

#include "utils/data.hpp"

#include "vis/kmeans.hpp"

BOOST_AUTO_TEST_CASE(test_quantize) {
    int dimension = 2;
    int numData = 100;
    arma::mat data = test::testData<double>(dimension, numData);

    int numClusters = 3;
    vis::KMeans<double> kmeans;
    arma::mat centers = kmeans.cluster(data, numClusters);

    BOOST_CHECK_EQUAL(dimension, centers.n_rows);
    BOOST_CHECK_EQUAL(numClusters, centers.n_cols);
}

