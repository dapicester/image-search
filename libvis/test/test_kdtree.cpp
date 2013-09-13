/**
 * @file test_kdtree.cpp
 * @brief Kd-tree test
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE kdtree
#include <boost/test/unit_test.hpp>

#include "kdtree.hpp"
#include "test_utils.hpp"

using cv::Mat;
using std::vector;
using namespace vis;

BOOST_AUTO_TEST_CASE(test_kdtree) {
    Mat data = getTestData<float>(1000, 2000);
    KDTree<float> tree(data);

    Mat query = data.row(0);
    vector<KDTreeNeighbor> results = tree.search(data);
    BOOST_CHECK_EQUAL(1, results.size());

    KDTreeNeighbor record = results.front();
    BOOST_CHECK(equals(query, data.row(record.index)));
}

