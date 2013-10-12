/**
 * @file test_standardize.cpp
 * @brief Test for function standardizeImage
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE standardize
#include <boost/test/unit_test.hpp>

#include "standardize.hpp"
#include "utils/print.hpp"
#include <opencv2/opencv.hpp>

using namespace vis;
using namespace cv;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

BOOST_AUTO_TEST_CASE(color_resize) {
    BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");

    Mat image = imread(argv[1]);

    double min, max;
    minMaxIdx(image, &min, &max);

    BOOST_CHECK_GT(image.size().height, standardize::HEIGHT);       // need to be resized
    BOOST_CHECK_EQUAL(image.type(), CV_8UC3);                       // 3 channels stored as unsigned bytes
    BOOST_CHECK_EQUAL(image.depth(), CV_8U);
    BOOST_CHECK(image.depth() == DataType<uchar>::type);            // the same but using templates
    BOOST_CHECK_EQUAL(image.channels(), 3);
    BOOST_CHECK_EQUAL(Vec3b(74, 111, 193), image.at<Vec3b>(0,0));   // NOTE format is BGR not RGB!
    BOOST_CHECK(  0u <= static_cast<uchar>(min));                   // range is [0, 255]
    BOOST_CHECK(255u >= static_cast<uchar>(max));

    Mat standard;
    standardizeImage(image, standard);
    minMaxIdx(standard, &min, &max);

    BOOST_CHECK_EQUAL(standard.size().height, standardize::HEIGHT); // resized
    BOOST_CHECK_EQUAL(standard.type(), CV_32FC3);                   // 3 channels stored as floats
    BOOST_CHECK_EQUAL(standard.depth(), CV_32F);
    BOOST_CHECK(standard.depth() == DataType<float>::type);
    BOOST_CHECK_EQUAL(standard.channels(), 3);
    BOOST_CHECK(0.0 <= min);                                        // range is [0, 1]
    BOOST_CHECK(1.0 >= max);

    if (argc > 2) {
        imshow("Input", image);
        imshow("Output", standard);

        print("Press a key to continue");
        waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(grayscale_noresize) {
    BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");

    const static int height = 1024;

    Mat image = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
    BOOST_CHECK_LT(image.size().height, height);                    // do not need to be resized
    BOOST_CHECK_EQUAL(image.channels(), 1);

    Mat standard = standardizeImage(image, height);
    BOOST_CHECK_NE(standard.size().height, height);                 // has not been resized
    BOOST_CHECK_EQUAL(standard.size(), image.size());
    BOOST_CHECK_EQUAL(standard.type(), CV_32FC1);
    BOOST_CHECK_EQUAL(standard.depth(), CV_32F);
    BOOST_CHECK_EQUAL(standard.channels(), 1);

    if (argc > 2) {
        imshow("Input", image);
        imshow("Output", standard);

        print("Press a key to continue");
        waitKey(0);
    }
}

