/**
 * @file opencv_usage.cpp
 * @brief Usage test for the OpenCV libraries
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE usage
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"

#include "utils/matrix.hpp"
#include "utils/print.hpp"

#include <opencv2/opencv.hpp>

#define argc boost::unit_test::framework::master_test_suite().argc

using namespace cv;

BOOST_AUTO_TEST_CASE(read_show) {
    Mat image = imread(test::LENA);
    BOOST_CHECK_MESSAGE(image.data, "No image data");

    BOOST_CHECK_EQUAL(Size(512, 512), image.size());
    BOOST_CHECK_EQUAL(512, image.rows);
    BOOST_CHECK_EQUAL(512, image.cols);
    BOOST_CHECK_EQUAL(2, image.dims);
    BOOST_CHECK_EQUAL(3, image.channels());

    if (argc > 1) {
        imshow("Display image", image);

        println("Press a key to continue");
        waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(opencv_traits) {
    float data[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Mat mat(3,3, CV_32F, data);

    BOOST_CHECK(CV_32F == DataType<float>::type);
    BOOST_CHECK(mat.depth() == DataType<float>::type);
}

BOOST_AUTO_TEST_CASE(matrix_random) {
    Mat data(10, 10, CV_32F);

    {
        // uniform distribution
        randu(data, Scalar(0), Scalar(1)); // min/max
        printmat(data);
        BOOST_CHECK(test::hasMinMax(data, 0., 1.));
    }
    {
        randn(data, Scalar(0), Scalar(1)); // mean/variance
        printmat(data);
    }
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
    BOOST_CHECK(test::equals(expected, mat));
}

BOOST_AUTO_TEST_CASE(matrix_split_merge) {
    vector<Mat> v;

    const int N = 10;
    for (int i = 0; i < N; i++) {
        Mat m = (Mat_<int>(2,2) << 1,2,3,4);
        v.push_back(m);
    }

    Mat out; // n-dimensional matrix
    merge(v, out);

    BOOST_CHECK_EQUAL(Size(2,2), out.size());
    BOOST_CHECK_EQUAL(N, out.channels());
}

BOOST_AUTO_TEST_CASE(matrix_multi) {
    int SIZE[] = { 2, 3, 2 }; // rows, columns, dimensions
    {
        Mat data(3, SIZE, DataType<int>::type, Scalar::all(0));
        BOOST_CHECK_EQUAL(3, data.dims);
        BOOST_CHECK_EQUAL(-1, data.rows);
        BOOST_CHECK_EQUAL(-1, data.cols);
        BOOST_CHECK_EQUAL(1, data.channels());
        BOOST_CHECK_EQUAL(Size(SIZE[1],SIZE[0]), data.size()); // NOTE this is not conformant to the API!
        //BOOST_CHECK_EQUAL(Size(-1, -1)), data.size());       //      this should be correct
        BOOST_CHECK_EQUAL(SIZE[0]*SIZE[1]*SIZE[2], data.total());
        BOOST_CHECK_EQUAL(SIZE[0]*SIZE[1], data.step1());

        // NOTE multidimensional matrix actually is a row matrix,
        // so one has to manually access elements using ptr() or at()

        size_t total = data.total(), step = data.step1();
        int n = 1;
        for (int i = 0; i < total; i += step) { // iterate on SIZE[2]
            for (int j = 0; j < step; j++) {    // iterate on SIZE[0]*SIZE[1]
                data.at<int>(i+j) = n;
            }
            n++;
        }

        // NOTE NAryMatIterator can also be used

        const Mat* array = &data;
        Mat plane;
        NAryMatIterator it(&array, &plane, 1);
        for (int p = 0; p < it.nplanes; p++) { // NOTE actually it.nplanes is the last argument to NAryMatIterator
            printmat(plane);
            BOOST_CHECK_EQUAL(Size(total,1), plane.size());
        }
    }
    {
        vector<Mat> data;
        for (int i = 0; i < SIZE[2]; i++) {
            data.push_back(Mat(SIZE[0], SIZE[1], DataType<int>::type, Scalar::all(i+1)));
        }

        for (int p = 0; p < data.size(); p++) {
            const Mat& plane = data[p];
            printmat(plane);
        }
    }
}

BOOST_AUTO_TEST_CASE(matrix_serialization) {
    Mat mat(4, 4, CV_32F);
    randu(mat, Scalar(0), Scalar(255));
    printmat(mat);

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
        printmat(loaded);
        BOOST_CHECK(test::equals(mat, loaded));

        fs.release();
        BOOST_CHECK(not fs.isOpened());
    }
}

BOOST_AUTO_TEST_CASE(hsv) {
    Mat image = imread(test::LENA);
    BOOST_CHECK_MESSAGE(image.data, "No image data");

    image.convertTo(image, CV_32F, 1./255);
    BOOST_CHECK_EQUAL(CV_32FC3, image.type());
    BOOST_CHECK(test::hasMinMax(image, 0., 1.));

    Mat hsv;
    cvtColor(image, hsv, CV_BGR2HSV);

    vector<Mat> planes;
    split(hsv, planes);

    Mat hue = planes[0];
    BOOST_CHECK(test::hasMinMax(hue, 0., 360.));    // NOTE hue is in [0,360]
    hue /= 360.;
    BOOST_CHECK(test::hasMinMax(hue, 0., 1.));

    Mat sat = planes[1];
    BOOST_CHECK(test::hasMinMax(sat, 0., 1.));

    Mat val = planes[2];
    BOOST_CHECK(test::hasMinMax(val, 0., 1.));

    Mat bgr;
    cvtColor(hsv, bgr, CV_HSV2BGR);

    if (argc > 2) {
        imshow("image", image);
        imshow("hsv", hsv);
        imshow("bgr", bgr);
        waitKey(0);
    }
}

