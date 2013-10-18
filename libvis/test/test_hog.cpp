/**
 * @file test_hog.cpp
 * @brief Test for function hogDescriptors
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hog
#include <boost/test/unit_test.hpp>

#include "hog.hpp"
#include "fixtures.hpp"
#include "utils/data.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include <opencv2/highgui/highgui.hpp>

using cv::Mat;
using cv::Size;

#define argc boost::unit_test::framework::master_test_suite().argc

BOOST_FIXTURE_TEST_CASE(compute_hog, test::Lena) {
    vis::HogExtractor extractor;

    vis::HogDescriptors descriptors = extractor.extract(image);
    BOOST_CHECK_EQUAL(descriptors.getWidth(), 16);
    BOOST_CHECK_EQUAL(descriptors.getHeight(), 16);
    BOOST_CHECK_EQUAL(descriptors.getDimension(), 3*8+4);

    Mat hogMatrix = descriptors.toMat();
    Size size = hogMatrix.size();
    BOOST_CHECK_EQUAL(Size(16*16, 28), size); // NOTE size is (cols, rows)
    BOOST_CHECK_EQUAL(28, hogMatrix.rows);
    BOOST_CHECK_EQUAL(16*16, hogMatrix.cols);

    // direct conversion to Mat
    Mat hogMatrix2 = extractor.extract(image).toMat();
    Mat diff = hogMatrix != hogMatrix2;
    BOOST_CHECK_EQUAL(0, countNonZero(diff));

    if (argc > 1) {
        cv::imshow("image", input);

        Mat hogImage = extractor.render(descriptors);
        cv::imshow("hog", hogImage);

        println("Press a key to continue");
        cv::waitKey(0);
    }
}

BOOST_FIXTURE_TEST_CASE(compute_hog_matrix, test::Lena) {
    vis::HogExtractor extractor;

    // descriptors and then matrix
    vis::HogDescriptors descriptors = extractor.extract(image);
    Mat hogMatrix = descriptors.toMat();

    // direct conversion to Mat
    Mat hogMatrix2 = extractor.extract(image).toMat();
    BOOST_CHECK(test::equals(hogMatrix, hogMatrix2));
}

BOOST_AUTO_TEST_CASE(hog_values) {
    vis::HogExtractor extractor;

    Mat hog = extractor.extract(Mat::eye(16, 16, CV_32F)).toMat();
    BOOST_CHECK_EQUAL(Size(4, 28), hog.size()); // NOTE size is (cols, rows)

    print(hog);

    // from Matlab
    Mat expected = (cv::Mat_<float>(28,4) <<
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.400000006, 0.185182467, 0.319459617, 0.400000006,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.400000006, 0.319459617, 0.185182467, 0.400000006,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0,
            0.400000006, 0.400000006, 0.400000006, 0.400000006,
            0.0, 0.0, 0.0, 0.0,
            0.047140457, 0.047140457, 0.047140457, 0.047140457,
            0.047140457, 0.047140457, 0.047140457, 0.047140457,
            0.047140457, 0.047140457, 0.047140457, 0.047140457,
            0.047140457, 0.047140457, 0.047140457, 0.047140457
        );

    print(expected);

    BOOST_REQUIRE_EQUAL(expected.size(), hog.size());
    for (int i = 0; i < expected.rows ; i++)
        for (int j = 0; j < expected.cols; j++)
            BOOST_REQUIRE_CLOSE_FRACTION(expected.at<float>(i, j), hog.at<float>(i,j), 1e-6);
}

