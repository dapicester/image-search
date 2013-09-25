/**
 * @file test_hog.cpp
 * @brief Test for function hogDescriptors
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE sanity
#include <boost/test/unit_test.hpp>

#include "hog.hpp"
#include "standardize.hpp"
#include "utils/matrix.hpp"
#include "utils/random.hpp"
#include "test_utils.hpp"
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
        standardizeImage(input, image);
    }
    ~Fixture() {}
    Mat input, image;
};

BOOST_FIXTURE_TEST_SUITE(suite, Fixture)

BOOST_AUTO_TEST_CASE(compute_hog) {
    HogExtractor extractor;
    HogDescriptors descriptors = extractor.extract(image);
    BOOST_CHECK_EQUAL(descriptors.width, 60);
    BOOST_CHECK_EQUAL(descriptors.height, 60);
    BOOST_CHECK_EQUAL(descriptors.dimension, 28);

    Mat hogMatrix = descriptors.toMat();
    Size size = hogMatrix.size();
    BOOST_CHECK_EQUAL(size, Size(28, 60*60));

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

