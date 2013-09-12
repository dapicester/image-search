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

using namespace cv;
using namespace std;

BOOST_AUTO_TEST_CASE(lena) {
    Mat image = imread(argv[1]);
    BOOST_WARN_MESSAGE(!image.data, "No image data");

    BOOST_CHECK_EQUAL(Size(512, 512), image.size());
    BOOST_CHECK_EQUAL(512, image.rows);
    BOOST_CHECK_EQUAL(512, image.cols);
    BOOST_CHECK_EQUAL(2, image.dims);
    BOOST_CHECK_EQUAL(3, image.channels());

    if (argc > 2) {
        imshow("Display image", image);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }
}

#define print(X) cout << #X << ": " << X << endl;

BOOST_AUTO_TEST_CASE(opencv_traits) {
    float data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Mat mat(3,3, CV_32F, data);

    BOOST_CHECK(CV_32F == DataType<float>::type);
    BOOST_CHECK(mat.depth() == DataType<float>::type);
}

