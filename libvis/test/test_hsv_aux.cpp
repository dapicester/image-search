/**
 * @file test_hsv_aux.cpp
 * @brief Test for HSV color histogram auxiliary functions
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hsv_aux
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "hsv_aux.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include <opencv2/highgui/highgui.hpp>

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

#define display(TITLE, IMAGE) \
    do { \
        cv::namedWindow((TITLE), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED); \
        cv::imshow((TITLE),(IMAGE)); \
    } while(0)


BOOST_AUTO_TEST_CASE(test_ind2sub) {
    const arma::ivec3 size = {3,2,2};

    std::vector<std::pair<arma::ivec3,int>> values = {
        std::make_pair(arma::ivec3{0,0,0}, 0),
        std::make_pair(arma::ivec3{2,0,0}, 2),
        std::make_pair(arma::ivec3{1,1,0}, 4),
        std::make_pair(arma::ivec3{2,0,1}, 8),
        std::make_pair(arma::ivec3{2,1,1}, 11),
    };

    for (auto p: values) {
        BOOST_CHECK(test::equals(p.first, vis::ind2sub(size, p.second)));
    }
}

cv::Mat swapChannels(const cv::Mat& in) {
    cv::Mat out(in.size(), in.type());
    const static int fromto[] = { 0,2,  1,1,  2,0 };
    cv::mixChannels(&in, 1, &out, 1, fromto, 3);
    return out;
}

BOOST_FIXTURE_TEST_CASE(test_functions, test::Peppers) {
    cv::Mat hsv = vis::toHsv(image);

    BOOST_CHECK_EQUAL(hsv.size(), image.size());
    BOOST_CHECK_EQUAL(hsv.type(), image.type());
    BOOST_CHECK(test::hasMinMax(hsv, 0., 1.));

    arma::ivec3 levels = { 3, 2+1, 2+1 };
    cv::Mat quantized = vis::quantize(hsv, levels);
    BOOST_CHECK_EQUAL(quantized.size(), image.size());
    BOOST_CHECK_EQUAL(quantized.type(), image.type());

    std::vector<cv::Mat> planes;
    cv::split(quantized, planes);
    BOOST_CHECK(test::hasMinMax(planes[0], 1, levels[0]));
    BOOST_CHECK(test::hasMinMax(planes[1], 1, levels[1]));
    BOOST_CHECK(test::hasMinMax(planes[2], 1, levels[2]));

    if (argc > 1) {
        display("image", image);
        display("hsv", swapChannels(hsv)); // HSV channels are interpreted as BGR

        planes[0] /= levels[0];
        planes[1] /= levels[1];
        planes[2] /= levels[2];
        cv::merge(planes, quantized);
        display("quantized", swapChannels(quantized)); // idem

        cv::Mat bgr = vis::toBgr(quantized);
        display("rgb", bgr);

        println("Press a key to continue");
        cv::waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(test_imquantize) {
    cv::Mat data = (cv::Mat_<int>(5,5) << 17, 23,  4, 10, 11,
                                          24,  5,  6, 12, 18,
                                           1,  7, 13, 19, 25,
                                           8, 14, 20, 21,  2,
                                          15, 16, 22,  3,  9);

    cv::Mat levels = (cv::Mat_<int>(1,5) << 1, 6, 11, 16, 21);

    cv::Mat expected = (cv::Mat_<int>(5,5) << 5, 6, 1, 3, 4,
                                              6, 2, 3, 4, 4,
                                              2, 2, 4, 5, 6,
                                              3, 4, 5, 5, 2,
                                              3, 5, 6, 2, 3);

    cv::Mat actual = vis::imquantize<int>(data.t(), levels); // NOTE transposed wrt Matlab

    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_medfilt2) {
    cv::Mat data = (cv::Mat_<float>(5,5) << 17, 23,  4, 10, 11,
                                            24,  5,  6, 12, 18,
                                             1,  7, 13, 19, 25,
                                             8, 14, 20, 21,  2,
                                            15, 16, 22,  3,  9);

    cv::Mat expected = (cv::Mat_<float>(5,5) << 17, 17,  8,  8, 15,
                                                17,  7,  8, 14, 16,
                                                10, 10, 13, 16, 16,
                                                10, 12, 18, 19,  9,
                                                11, 18, 18,  9,  9);

    cv::Mat actual = vis::medfilt2(data.t()); // NOTE transposed wrt Matlab

    printmat(data);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_linspace) {
    {
        arma::fvec expected = {1,2,3,4,5,6,7,8,9,10};
        arma::fvec actual = arma::linspace<arma::fvec>(1., 10., 10);
        BOOST_CHECK(test::equals(expected, actual));
    }
    {
        arma::vec expected = { 1.0, 1.642857143, 2.285714286, 2.928571429, 3.571428571,
                               4.214285714, 4.857142857, 5.5, 6.142857143, 6.785714286,
                               7.428571429, 8.071428571, 8.714285714, 9.357142857, 10.0 };
        arma::vec actual = arma::linspace<arma::vec>(1, 10, 15);
        BOOST_REQUIRE_EQUAL(expected.size(), actual.size());
        for(int i = 0; i < 15; ++i) BOOST_CHECK_CLOSE(expected(i), actual(i), 0.000001);
    }
}

template <typename T>
void doReshapeTest(const arma::Mat<T>& expected, const arma::Mat<T>& actual) {
    printmat(expected);
    printmat(actual);

    BOOST_CHECK_EQUAL(actual.size(), expected.size());
    BOOST_CHECK_EQUAL(actual.n_rows, expected.n_rows);
    BOOST_CHECK_EQUAL(actual.n_cols, expected.n_cols);
    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_reshape) {
    arma::imat data = { 8, 1, 6, 2,
                        3, 5, 7, 9,
                        4, 9, 2, 1 };
    data = arma::reshape(data,4,3).t(); // NOTE column-major vs row-major
    printmat(data);

    {
        arma::imat expected = { 8, 4, 5, 6, 2, 9,
                                3, 1, 9, 7, 2, 1 };
        expected = arma::reshape(expected,6,2).t();

        arma::imat actual(data);
        actual.reshape(2,6);
        doReshapeTest(expected, actual);
    }
    {
        arma::imat expected = { 8, 6,
                                3, 7,
                                4, 2,
                                1, 2,
                                5, 9,
                                9, 1 };
        expected = arma::reshape(expected,2,6).t();

        arma::imat actual(data);
        actual.reshape(6,2);
        doReshapeTest(expected, actual);
    }
}

