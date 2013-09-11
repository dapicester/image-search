#define BOOST_TEST_MODULE linspace
#include <boost/test/unit_test.hpp>

#include "linspace.hpp"
#include "test_utils.hpp"

using namespace cv;

BOOST_AUTO_TEST_CASE(linspace_implicit) {
    Mat expected = (Mat_<double>(1,10) << 1,2,3,4,5,6,7,8,9,10);
    Mat actual = vis::linspace(1.0, 10.0, 10);
    BOOST_CHECK(equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(linspace_int) {
    Mat expected = (Mat_<int>(1,10) << 1,2,3,4,5,6,7,8,9,10);
    Mat actual = vis::linspace<int>(1, 10, 10);
    BOOST_CHECK(equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(linspace_double) {
    Mat expected = (Mat_<double>(1,15)
            << 1.0, 1.642857143, 2.285714286, 2.928571429, 3.571428571,
               4.214285714, 4.857142857, 5.5, 6.142857143, 6.785714286,
               7.428571429, 8.071428571, 8.714285714, 9.357142857, 10.0);
    Mat actual = vis::linspace<double>(1, 10, 15);
    BOOST_REQUIRE_EQUAL(expected.size(), actual.size());

    for(int i = 0; i < 15; ++i) {
        BOOST_CHECK_CLOSE(expected.at<double>(i), actual.at<double>(i), 0.000001);
    }
}

