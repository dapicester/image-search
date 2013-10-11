#define BOOST_TEST_MODULE utils_ported
#include <boost/test/unit_test.hpp>

#include "utils/ported.hpp"
#include "utils/data.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

BOOST_AUTO_TEST_CASE(test_colon_indices) {
    Mat expected, actual, indices;
    Mat input = (Mat_<int>(3, 4) << 1, 2, 3, 4,
                                    5, 6, 7, 8,
                                    9, 10, 11, 12);

    /* column subset */
    expected = (Mat_<int>(3, 2) << 1, 3,
                                   5, 7,
                                   9, 11);
    indices = (Mat_<int>(1,2) << 0, 2);
    actual = colon<int>(input, indices, COLUMNS);

    printmat(input);
    printmat(indices);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(equals(expected, actual));
    BOOST_CHECK(equals(expected, colon<int>(input, indices.t(), COLUMNS)));

    /* row subset */
    expected = (Mat_<int>(2, 4) << 1, 2, 3, 4,
                                   5, 6, 7, 8);
    indices = (Mat_<int>(1, 2) << 0, 1);
    actual = colon<int>(input, indices.t(), ROWS);

    printmat(input);
    printmat(indices);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(equals(expected, actual));
    BOOST_CHECK(equals(expected, colon<int>(input, indices.t(), ROWS)));
}

BOOST_AUTO_TEST_CASE(test_colon) {
    Mat expected, actual;
    Mat input = (Mat_<int>(3, 4) << 1, 2, 3, 4,
                                    5, 6, 7, 8,
                                    9, 10, 11, 12);

    expected = (Mat_<int>(1, 3*4) << 1, 2, 3, 4,
                                     5, 6, 7, 8,
                                     9, 10, 11, 12);
    actual = colon(input);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_linspace) {
    Mat expected, actual;

    /* implicit type */
    expected = (Mat_<double>(1,10) << 1,2,3,4,5,6,7,8,9,10);
    actual = vis::linspace(1.0, 10.0, 10);
    BOOST_CHECK(equals(expected, actual));

    /* explicit type */
    expected = (Mat_<int>(1,10) << 1,2,3,4,5,6,7,8,9,10);
    actual = vis::linspace<int>(1, 10, 10);
    BOOST_CHECK(equals(expected, actual));

    /* double */
    expected = (Mat_<double>(1,15)
            << 1.0, 1.642857143, 2.285714286, 2.928571429, 3.571428571,
               4.214285714, 4.857142857, 5.5, 6.142857143, 6.785714286,
               7.428571429, 8.071428571, 8.714285714, 9.357142857, 10.0);
    actual = vis::linspace<double>(1, 10, 15);
    BOOST_REQUIRE_EQUAL(expected.size(), actual.size());

    for(int i = 0; i < 15; ++i) {
        BOOST_CHECK_CLOSE(expected.at<double>(i), actual.at<double>(i), 0.000001);
    }
}

BOOST_AUTO_TEST_CASE(test_round) {
    BOOST_CHECK_EQUAL(1, vis::iround(1.0));
    BOOST_CHECK_EQUAL(1, vis::iround(1.1));
    BOOST_CHECK_EQUAL(2, vis::iround(1.5));
    BOOST_CHECK_EQUAL(2, vis::iround(1.8));
    BOOST_CHECK_EQUAL(2, vis::iround(2.0));

    BOOST_CHECK_EQUAL(1.0, vis::round(1.0));
    BOOST_CHECK_EQUAL(1.0, vis::round(1.1));
    BOOST_CHECK_EQUAL(2.0, vis::round(1.5));
    BOOST_CHECK_EQUAL(2.0, vis::round(1.8));
    BOOST_CHECK_EQUAL(2.0, vis::round(2.0));

    Mat input = (Mat_<double>(1, 5) << 1.0, 1.5, 2.7, 3.0, 4.99);
    Mat expected = (Mat_<double>(1, 5) << 1, 2, 3, 3, 5);
    Mat actual = vis::round<double>(input);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_colsubset) {
    Mat input = (Mat_<double>(1,10) << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    Mat expected = (Mat_<double>(1,5) << 1, 3, 6, 8, 10);

    Mat actual = colsubset<double>(input, 5, UNIFORM);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(equals(expected, actual));
}

void doReshapeTest(const Mat& expected, const Mat& actual) {
    printmat(expected);
    printmat(actual);

    BOOST_CHECK_EQUAL(actual.size(), expected.size());
    BOOST_CHECK(vis::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_reshape) {
    Mat data = (Mat_<int>(3,4) << 8, 1, 6, 2,
                                  3, 5, 7, 9,
                                  4, 9, 2, 1);
    printmat(data);
    {
        Mat expected = (Mat_<int>(2,6) << 8, 4, 5, 6, 2, 9,
                                          3, 1, 9, 7, 2, 1);

        Mat actual = reshape(data,6);
        doReshapeTest(expected, actual);
    }
    {
        Mat expected = (Mat_<int>(6,2) << 8, 6,
                                          3, 7,
                                          4, 2,
                                          1, 2,
                                          5, 9,
                                          9, 1);
        Mat actual = reshape(data,2);
        doReshapeTest(expected, actual);
    }
}

BOOST_AUTO_TEST_CASE(test_hist) {
    {
        // random data
        int numdata = 50;
        Mat data(1, numdata, cv::DataType<float>::type);
        cv::randn(data, cv::Scalar(0), cv::Scalar(1));

        int numbins = 10;
        Mat histogram = hist(data, numbins);
        double sum = cv::sum(histogram)[0];

        printmat(histogram);
        print(sum);

        BOOST_CHECK_EQUAL(cv::Size(1, numbins), histogram.size());
        BOOST_CHECK_EQUAL(numdata, sum);

        Mat normalized = hist(data, numbins, SUM1);
        Mat expected = histogram / sum;

        printmat(normalized);
        printmat(expected);

        BOOST_CHECK_CLOSE(1., cv::sum(normalized)[0], 1e-5);
        BOOST_CHECK_CLOSE(1., cv::sum(expected)[0], 1e-5);
        BOOST_CHECK(equals(expected, normalized));
    }
    {
        // matlab compatibility
        Mat data = (cv::Mat_<float>(25,1) << 17, 23,  4, 10, 11,
                                           24,  5,  6, 12, 18,
                                            1,  7, 13, 19, 25,
                                            8, 14, 20, 21,  2,
                                           15, 16, 22,  3,  9);
        Mat expected = (cv::Mat_<float>(10,1) << 3, 2, 3, 2, 3, 2, 2, 3, 2, 3);
        Mat actual = hist(data, 10);

        printmat(expected);
        printmat(actual);

        BOOST_CHECK(equals(expected, actual));
    }

}

