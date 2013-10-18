/**
 * @file test_standardize.cpp
 * @brief Test for function standardizeImage
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE standardize
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "standardize.hpp"
#include "utils/print.hpp"
#include <opencv2/opencv.hpp>

using cv::Mat;

#define argc boost::unit_test::framework::master_test_suite().argc

BOOST_AUTO_TEST_CASE(color_resize) {
    Mat image = cv::imread(test::LENA);
    BOOST_REQUIRE_MESSAGE(image.data, "Require lena image");

    double min, max;
    cv::minMaxIdx(image, &min, &max);

    BOOST_CHECK_GT(image.size().height, vis::standardize::HEIGHT);       // need to be resized
    BOOST_CHECK_EQUAL(image.type(), CV_8UC3);                       // 3 channels stored as unsigned bytes
    BOOST_CHECK_EQUAL(image.depth(), CV_8U);
    BOOST_CHECK(image.depth() == cv::DataType<uchar>::type);            // the same but using templates
    BOOST_CHECK_EQUAL(image.channels(), 3);
    BOOST_CHECK_EQUAL(cv::Vec3b(74, 111, 193), image.at<cv::Vec3b>(0,0));   // NOTE format is BGR not RGB!
    BOOST_CHECK(  0u <= static_cast<uchar>(min));                   // range is [0, 255]
    BOOST_CHECK(255u >= static_cast<uchar>(max));

    Mat standard;
    vis::standardizeImage(image, standard);
    cv::minMaxIdx(standard, &min, &max);

    BOOST_CHECK_EQUAL(standard.size().height, vis::standardize::HEIGHT); // resized
    BOOST_CHECK_EQUAL(standard.type(), CV_32FC3);                   // 3 channels stored as floats
    BOOST_CHECK_EQUAL(standard.depth(), CV_32F);
    BOOST_CHECK(standard.depth() == cv::DataType<float>::type);
    BOOST_CHECK_EQUAL(standard.channels(), 3);
    BOOST_CHECK(0.0 <= min);                                        // range is [0, 1]
    BOOST_CHECK(1.0 >= max);

    if (argc > 1) {
        cv::imshow("Input", image);
        cv::imshow("Output", standard);

        println("Press a key to continue");
        cv::waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(grayscale_noresize) {
    const static int height = 1024;

    Mat image = cv::imread(test::LENA, CV_LOAD_IMAGE_GRAYSCALE);
    BOOST_REQUIRE_MESSAGE(image.data, "Require lena image");

    BOOST_CHECK_LT(image.size().height, height);                    // do not need to be resized
    BOOST_CHECK_EQUAL(image.channels(), 1);

    Mat standard = vis::standardizeImage(image, height);
    BOOST_CHECK_NE(standard.size().height, height);                 // has not been resized
    BOOST_CHECK_EQUAL(standard.size(), image.size());
    BOOST_CHECK_EQUAL(standard.type(), CV_32FC1);
    BOOST_CHECK_EQUAL(standard.depth(), CV_32F);
    BOOST_CHECK_EQUAL(standard.channels(), 1);

    if (argc > 1) {
        cv::imshow("Input", image);
        cv::imshow("Output", standard);

        println("Press a key to continue");
        cv::waitKey(0);
    }
}

