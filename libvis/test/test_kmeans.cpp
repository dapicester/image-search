/**
 * @file test_kmeans.cpp
 * @brief K-means test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kmeans
#include <boost/test/unit_test.hpp>

#include "kmeans.hpp"
#include "test_utils.hpp"

using cv::Mat;
using namespace vis;

BOOST_AUTO_TEST_CASE(clusters) {
    Mat data = getTestData<float>(256, 10000);
    VlKMeans* centers = kmeans<float>(data, 40);
    vl_kmeans_delete(centers);
}

