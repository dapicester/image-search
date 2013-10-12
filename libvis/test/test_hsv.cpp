/**
 * @file test_hsv.cpp
 * @brief Test for HSV color histogram
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hsv
#include <boost/test/unit_test.hpp>

#include "hsv.hpp"
#include "standardize.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace vis;
using namespace cv;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

#define display(IMAGE) namedWindow((#IMAGE)); imshow((#IMAGE),(IMAGE));

struct Fixture {
    Fixture() {
        BOOST_REQUIRE_MESSAGE(argc > 1, "Require lena image");

        input = imread(argv[1]);
        standardizeImage(input, image, 128);
    }
    ~Fixture() {}
    Mat input, image;
};

BOOST_FIXTURE_TEST_SUITE(suite, Fixture)

BOOST_AUTO_TEST_CASE(test_functions) {
    Mat hsv = toHsv(image);

    BOOST_CHECK_EQUAL(hsv.size(), image.size());
    BOOST_CHECK_EQUAL(hsv.type(), image.type());
    BOOST_CHECK(hasMinMax(hsv, 0., 1.));

    Vec3i levels(3, 2, 2);
    Mat quantized = quantize(hsv, levels);
    BOOST_CHECK_EQUAL(quantized.size(), image.size());
    BOOST_CHECK_EQUAL(quantized.type(), image.type());

    if (argc > 2) {
        display(image);
        display(hsv);
        display(quantized);

        print("Press a key to continue");
        waitKey(0);
    }
}

BOOST_AUTO_TEST_SUITE_END()

