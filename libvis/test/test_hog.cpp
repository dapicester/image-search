/**
 * @file test_hog.cpp
 * @brief Test for function hogDescriptors
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hog
#include <boost/test/unit_test.hpp>

#include "hog.hpp"
#include "standardize.hpp"
#include "utils/data.hpp"
#include "utils/matrix.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace vis;
using namespace cv;
using namespace std;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

struct Fixture {
    Fixture() {
        BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");

        input = imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
        standardizeImage(input, image, 128);
    }
    ~Fixture() {}
    Mat input, image;
};

BOOST_FIXTURE_TEST_SUITE(suite, Fixture)

BOOST_AUTO_TEST_CASE(compute_hog) {
    HogExtractor extractor;

    ::HogDescriptors descriptors = extractor.extract(image);
    BOOST_CHECK_EQUAL(descriptors.getWidth(), 16);
    BOOST_CHECK_EQUAL(descriptors.getHeight(), 16);
    BOOST_CHECK_EQUAL(descriptors.getDimension(), 3*8+4);

    Mat hogMatrix = descriptors.toMat();
    Size size = hogMatrix.size();
    BOOST_CHECK_EQUAL(Size(16*16, 28), size); // XXX size is (cols, rows)
    BOOST_CHECK_EQUAL(28, hogMatrix.rows);
    BOOST_CHECK_EQUAL(16*16, hogMatrix.cols);

    // direct conversion to Mat
    Mat hogMatrix2 = extractor.extract(image).toMat();
    Mat diff = hogMatrix != hogMatrix2;
    BOOST_CHECK_EQUAL(0, countNonZero(diff));

    if (argc > 2) {
        namedWindow("image");
        imshow("image", input);

        Mat hogImage = extractor.render(descriptors);
        namedWindow("hog");
        imshow("hog", hogImage);

        cout << "Press a key to continue" << endl;
        waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(compute_hog_matrix) {
    HogExtractor extractor;

    // descriptors and then matrix
    HogDescriptors descriptors = extractor.extract(image);
    Mat hogMatrix = descriptors.toMat();

    // direct conversion to Mat
    Mat hogMatrix2 = extractor.extract(image).toMat();
    BOOST_CHECK(equals(hogMatrix, hogMatrix2));
}

BOOST_AUTO_TEST_SUITE_END()

#define PRINT 0

BOOST_AUTO_TEST_CASE(hog_values) {
    HogExtractor extractor;

    Mat hog = extractor.extract(Mat::eye(16, 16, CV_32F)).toMat();
    BOOST_CHECK_EQUAL(Size(4, 28), hog.size()); // XXX size is (cols, rows)
#if PRINT
    cout << "hog:\n" << hog << endl;
#endif

    // from Matlab
    Mat expected = (Mat_<float>(28,4) <<
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
#if PRINT
    cout << "expected:\n" << expected << endl;
#endif

    BOOST_REQUIRE_EQUAL(expected.size(), hog.size());
    for (int i = 0; i < expected.rows ; i++)
        for (int j = 0; j < expected.cols; j++)
            BOOST_REQUIRE_CLOSE_FRACTION(expected.at<float>(i, j), hog.at<float>(i,j), 1e-6);
}

