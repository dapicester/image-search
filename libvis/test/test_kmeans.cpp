/**
 * @file test_kmeans.cpp
 * @brief K-means test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kmeans
#include <boost/test/unit_test.hpp>

#include "kmeans.hpp"
#include "utils/data.hpp"

using cv::Mat;
using vis::KMeans;

BOOST_AUTO_TEST_CASE(test_quantize) {
    int dimension = 2;
    int numData = 100;
    Mat data = test::getTestData<double>(dimension, numData);

    int numClusters = 3;
    KMeans<double> kmeans;
    Mat centers = kmeans.cluster(data, numClusters);

    BOOST_CHECK_EQUAL(dimension, centers.rows);
    BOOST_CHECK_EQUAL(numClusters, centers.cols);
}

