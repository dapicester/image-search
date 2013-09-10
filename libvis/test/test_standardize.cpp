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

BOOST_AUTO_TEST_CASE(standardize) {
    Mat image = imread(argv[1]);
    Mat standard;

    standardizeImage(image, standard, 128);
    assert(standard.depth() == CV_32F);
    assert(standard.size().height == 128);

    if (argc > 2) {
        imshow("Input", image);
        imshow("Output", standard);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }
}

