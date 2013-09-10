/**
 * @file test_standardize.cpp
 * @brief Test for function standardizeImage
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE standardize
#include <boost/test/unit_test.hpp>

#include "standardize.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

struct Fixture {
    Fixture() { image = imread(argv[1]); }
    ~Fixture() {}
    Mat image;
    Mat standard;
};

BOOST_FIXTURE_TEST_SUITE(suite, Fixture)

BOOST_AUTO_TEST_CASE(standardize) {
    const static int STD_HEIGHT = 128;
    BOOST_CHECK(image.size().height > STD_HEIGHT);

    standardizeImage(image, standard, STD_HEIGHT);
    BOOST_CHECK(standard.depth() == CV_32F);
    BOOST_CHECK(standard.size().height == STD_HEIGHT);

    if (argc > 2) {
        imshow("Input", image);
        imshow("Output", standard);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(no_resize) {
    const static int STD_HEIGHT = 1024;
    BOOST_CHECK(image.size().height < STD_HEIGHT);

    standardizeImage(image, standard, STD_HEIGHT);
    BOOST_CHECK(standard.depth() == CV_32F);
    BOOST_CHECK(standard.size().height != STD_HEIGHT);
    BOOST_CHECK(standard.size() == image.size());
}

BOOST_AUTO_TEST_SUITE_END()

