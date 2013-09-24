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

#define BOOST_CHECK_EQ(X,Y) BOOST_CHECK_EQUAL(X,Y)
#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

BOOST_AUTO_TEST_CASE(color_resize) {
    BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");

    Mat image = imread(argv[1]);
    BOOST_CHECK_GT(image.size().height, IMG_HEIGHT);                // need to be resized
    BOOST_CHECK_EQ(image.type(), CV_8UC3);                          // 3 channels stored as unsigned bytes
    BOOST_CHECK_EQ(image.depth(), CV_8U);
    BOOST_CHECK(image.depth() == DataType<uchar>::type);            // the same but using templates
    BOOST_CHECK_EQ(image.channels(), 3);
    BOOST_CHECK_EQ(Vec3b(74, 111, 193), image.at<Vec3b>(0,0));      // REMIND: format is BGR not RGB!

    Mat standard;
    standardizeImage(image, standard);
    BOOST_CHECK_EQ(standard.size().height, IMG_HEIGHT);             // resized
    BOOST_CHECK_EQ(standard.type(), CV_32FC3);                      // 3 channels stored as floats
    BOOST_CHECK_EQ(standard.depth(), CV_32F);
    BOOST_CHECK(standard.depth() == DataType<float>::type);
    BOOST_CHECK_EQ(standard.channels(), 3);

    Vec3f expected(0.292769909, 0.437867939, 0.759436607);
    Vec3f actual = standard.at<Vec3f>(0,0);
    for (int i = 0; i < 3; i++) {
        BOOST_CHECK_CLOSE_FRACTION(expected[i], actual[i], 1e-5);
    }

    if (argc > 2) {
        imshow("Input", image);
        imshow("Output", standard);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(grayscale_noresize) {
    BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");

    const static int height = 1024;

    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    BOOST_CHECK_LT(image.size().height, height);                    // do not need to be resized
    BOOST_CHECK_EQ(image.channels(), 1);

    Mat standard;
    standardizeImage(image, standard, height);
    BOOST_CHECK_NE(standard.size().height, height);                 // has not been resized
    BOOST_CHECK_EQ(standard.size(), image.size());
    BOOST_CHECK_EQ(standard.type(), CV_32FC1);
    BOOST_CHECK_EQ(standard.depth(), CV_32F);
    BOOST_CHECK_EQ(standard.channels(), 1);
}

