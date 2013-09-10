/**
 * @file opencv_sanity.cpp
 * @brief Sanity test for the OpenCV libraries
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE sanity
#include <boost/test/unit_test.hpp>

#include <opencv2/opencv.hpp>
#include <iostream>

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

BOOST_AUTO_TEST_CASE(sanity) {
    cv::Mat image = cv::imread(argv[1]);
    BOOST_WARN_MESSAGE(!image.data, "No image data");

    if (argc > 2) {
        cv::imshow("Display image", image);

        std::cout << "Press a key to continue" << std::endl;
        cv::waitKey(0);
    }
}

