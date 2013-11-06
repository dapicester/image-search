#define BOOST_TEST_MODULE utils_ported
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "utils/ported.hpp"
#include "utils/data.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include <boost/lexical_cast.hpp>

using cv::DataType;
using cv::Mat;
using cv::Mat_;
using cv::Vec3i;

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
    actual = vis::colon<int>(input, indices, vis::COLUMNS);

    printmat(input);
    printmat(indices);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
    BOOST_CHECK(test::equals(expected, vis::colon<int>(input, indices.t(), vis::COLUMNS)));

    /* row subset */
    expected = (Mat_<int>(2, 4) << 1, 2, 3, 4,
                                   5, 6, 7, 8);
    indices = (Mat_<int>(1, 2) << 0, 1);
    actual = vis::colon<int>(input, indices.t(), vis::ROWS);

    printmat(input);
    printmat(indices);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
    BOOST_CHECK(test::equals(expected, vis::colon<int>(input, indices.t(), vis::ROWS)));
}

BOOST_AUTO_TEST_CASE(test_colon) {
    Mat expected, actual;
    Mat input = (Mat_<int>(3, 4) << 1, 2, 3, 4,
                                    5, 6, 7, 8,
                                    9, 10, 11, 12);

    expected = (Mat_<int>(1, 3*4) << 1, 2, 3, 4,
                                     5, 6, 7, 8,
                                     9, 10, 11, 12);
    actual = vis::colon(input);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_linspace) {
    Mat expected, actual;

    /* implicit type */
    expected = (Mat_<double>(1,10) << 1,2,3,4,5,6,7,8,9,10);
    actual = vis::linspace(1.0, 10.0, 10);
    BOOST_CHECK(test::equals(expected, actual));

    /* explicit type */
    expected = (Mat_<int>(1,10) << 1,2,3,4,5,6,7,8,9,10);
    actual = vis::linspace<int>(1, 10, 10);
    BOOST_CHECK(test::equals(expected, actual));

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

    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_colsubset) {
    Mat input = (Mat_<double>(1,10) << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    Mat expected = (Mat_<double>(1,5) << 1, 3, 6, 8, 10);

    Mat actual = vis::colsubset<double>(input, 5, vis::UNIFORM);

    printmat(input);
    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_subset) {
    std::vector<std::string> v;
    for (int i=1; i<=100; i++)
        v.push_back(boost::lexical_cast<std::string>(i));
    print(v);

    std::vector<std::string> actual = vis::subset(v, 10, vis::UNIFORM);
    print(actual);

    std::vector<std::string> expected;
    expected.push_back("1");
    expected.push_back("12");
    expected.push_back("23");
    expected.push_back("34");
    expected.push_back("45");
    expected.push_back("56");
    expected.push_back("67");
    expected.push_back("78");
    expected.push_back("89");
    expected.push_back("100");
    print(expected);

    BOOST_CHECK(std::equal(actual.begin(), actual.end(), expected.begin()));
}

void doReshapeTest(const Mat& expected, const Mat& actual) {
    printmat(expected);
    printmat(actual);

    BOOST_CHECK_EQUAL(actual.size(), expected.size());
    BOOST_CHECK(test::equals(expected, actual));
}

BOOST_AUTO_TEST_CASE(test_reshape) {
    Mat data = (Mat_<int>(3,4) << 8, 1, 6, 2,
                                  3, 5, 7, 9,
                                  4, 9, 2, 1);
    printmat(data);
    {
        Mat expected = (Mat_<int>(2,6) << 8, 4, 5, 6, 2, 9,
                                          3, 1, 9, 7, 2, 1);

        Mat actual = vis::reshape(data,6);
        doReshapeTest(expected, actual);
    }
    {
        Mat expected = (Mat_<int>(6,2) << 8, 6,
                                          3, 7,
                                          4, 2,
                                          1, 2,
                                          5, 9,
                                          9, 1);
        Mat actual = vis::reshape(data,2);
        doReshapeTest(expected, actual);
    }
}

BOOST_AUTO_TEST_CASE(test_hist) {
    {
        // random data
        int numdata = 50;
        Mat data(1, numdata, DataType<float>::type);
        cv::randn(data, cv::Scalar(0), cv::Scalar(1));

        int numbins = 10;
        Mat histogram = vis::hist(data, numbins);
        double sum = cv::sum(histogram)[0];

        printmat(histogram);
        print(sum);

        BOOST_CHECK_EQUAL(cv::Size(1, numbins), histogram.size());
        BOOST_CHECK_EQUAL(numdata, sum);

        Mat normalized = vis::hist(data, numbins, vis::SUM1);
        Mat expected = histogram / sum;

        printmat(normalized);
        printmat(expected);

        BOOST_CHECK_CLOSE(1., cv::sum(normalized)[0], 1e-5);
        BOOST_CHECK_CLOSE(1., cv::sum(expected)[0], 1e-5);
        BOOST_CHECK(test::equals(expected, normalized));
    }
    {
        // matlab compatibility
        Mat data = (Mat_<float>(25,1) << 17, 23,  4, 10, 11,
                                             24,  5,  6, 12, 18,
                                              1,  7, 13, 19, 25,
                                              8, 14, 20, 21,  2,
                                             15, 16, 22,  3,  9);
        Mat expected = (Mat_<float>(10,1) << 3, 2, 3, 2, 3, 2, 2, 3, 2, 3);
        Mat actual = vis::hist(data, 10);

        printmat(expected);
        printmat(actual);

        BOOST_CHECK(test::equals(expected, actual));
    }
}

BOOST_AUTO_TEST_CASE(test_imquantize) {
    Mat data = (Mat_<int>(5,5) << 17, 23,  4, 10, 11,
                                  24,  5,  6, 12, 18,
                                   1,  7, 13, 19, 25,
                                   8, 14, 20, 21,  2,
                                  15, 16, 22,  3,  9);

    Mat levels = (Mat_<int>(1,5) << 1, 6, 11, 16, 21);

    Mat expected = (Mat_<int>(5,5) << 5, 6, 1, 3, 4,
                                      6, 2, 3, 4, 4,
                                      2, 2, 4, 5, 6,
                                      3, 4, 5, 5, 2,
                                      3, 5, 6, 2, 3);

    Mat actual = vis::imquantize<int>(data.t(), levels); // NOTE transposed wrt Matlab

    printmat(expected);
    printmat(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

#if 0
typedef int(*func)(int, int, int);

void testOrder(func f, int rows, int cols, int* expected, int num) {
    for (int i = 0; i < num; i++)
        BOOST_CHECK_EQUAL(expected[i], f(i, rows, cols));
}

BOOST_AUTO_TEST_CASE(test_rc) {
    int rows = 3, cols = 2;
    int col2row[] = {0,3,1,4,2,5};
    int row2col[] = {0,2,4,1,3,5};

    testOrder(vis::c2r, rows, cols, col2row, 6);
    testOrder(vis::r2c, rows, cols, row2col, 6);
}
#endif

#if 0
int at(const Mat& mat, const Vec3i& index) {
    int idx[3];
    for (int i=0; i<3;i++) idx[i] = index[i];
    return mat.at<int>(idx) ;
}
#endif

BOOST_AUTO_TEST_CASE(test_ind2sub) {
    {
        const Vec3i size(3,2,2);

        // matlab-compatible indexing
        BOOST_CHECK_EQUAL(Vec3i(0,0,0), vis::ind2sub(size, 0));
        BOOST_CHECK_EQUAL(Vec3i(2,0,0), vis::ind2sub(size, 2));
        BOOST_CHECK_EQUAL(Vec3i(1,1,0), vis::ind2sub(size, 4));
        BOOST_CHECK_EQUAL(Vec3i(2,0,1), vis::ind2sub(size, 8));
        BOOST_CHECK_EQUAL(Vec3i(2,1,1), vis::ind2sub(size, 11));
    }
    {
        int sz[] = {3,2,2};
        int data[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
        Mat mat(3, sz, DataType<int>::type, data);
        printMat3(mat, sz, true);

#if 0
        // TODO OpenCV row-major order
        BOOST_CHECK_EQUAL(0, mat.at<int>(0,0,0));
        BOOST_CHECK_EQUAL(2, mat.at<int>(2,0,0));
        BOOST_CHECK_EQUAL(4, mat.at<int>(1,1,0));
        BOOST_CHECK_EQUAL(8, mat.at<int>(2,0,1));
        BOOST_CHECK_EQUAL(11, mat.at<int>(2,1,1));
#endif
    }
}

BOOST_AUTO_TEST_CASE(test_medfilt2) {
    Mat data = (Mat_<float>(5,5) << 17, 23,  4, 10, 11,
                                    24,  5,  6, 12, 18,
                                     1,  7, 13, 19, 25,
                                     8, 14, 20, 21,  2,
                                    15, 16, 22,  3,  9);

    Mat expected = (Mat_<float>(5,5) << 17, 17,  8,  8, 15,
                                        17,  7,  8, 14, 16,
                                        10, 10, 13, 16, 16,
                                        10, 12, 18, 19,  9,
                                        11, 18, 18,  9,  9);

    Mat actual = vis::medfilt2<float>(data.t()); // NOTE transposed wrt Matlab

    print(data);
    print(expected);
    print(actual);

    BOOST_CHECK(test::equals(expected, actual));
}

