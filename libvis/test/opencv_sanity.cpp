/**
 * @file opencv_sanity.cpp
 * @brief Sanity test for the OpenCV libraries
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE sanity
#include <boost/test/unit_test.hpp>

#include "utils/matrix.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

using namespace cv;
using namespace std;

BOOST_AUTO_TEST_CASE(lena) {
    BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");
    Mat image = imread(argv[1]);
    BOOST_CHECK_MESSAGE(image.data, "No image data");

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

BOOST_AUTO_TEST_CASE(matrix_push_back) {
    Mat row = Mat::ones(2, 3, CV_32F);

    Mat mat;
    for (int i = 1; i <= 3; i++) {
        mat.push_back( Mat(row*i) );
    }
    Mat expected = (Mat_<float>(6, 3) << 1, 1, 1,
                                         1, 1, 1,
                                         2, 2, 2,
                                         2, 2, 2,
                                         3, 3, 3,
                                         3, 3, 3);
    BOOST_CHECK(vis::equals(expected, mat));
}

BOOST_AUTO_TEST_CASE(matrix_serialization) {
    Mat mat(4, 4, CV_32F);
    randu(mat, Scalar(0), Scalar(255));
    cout << mat << endl;

    {
        FileStorage fs("test_serialization.yml", FileStorage::WRITE);
        BOOST_CHECK(fs.isOpened());

        fs << "matrix" << mat;

        fs.release();
        BOOST_CHECK(not fs.isOpened());
    }

    {
        FileStorage fs;
        fs.open("test_serialization.yml", FileStorage::READ);
        BOOST_CHECK(fs.isOpened());

        Mat loaded;
        fs["matrix"] >> loaded;
        cout << loaded << endl;
        BOOST_CHECK(vis::equals(mat, loaded));

        fs.release();
        BOOST_CHECK(not fs.isOpened());
    }
}

