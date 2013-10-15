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

Mat swapChannels(const Mat& in) {
    Mat out(in.rows, in.cols, in.type());
    static int fromto[] = { 0,2,  1,1,  2,0 };
    mixChannels(&in, 1, &out, 1, fromto, 3);
    return out;
}

BOOST_FIXTURE_TEST_SUITE(suite, Fixture)

BOOST_AUTO_TEST_CASE(test_functions) {
    Mat hsv = toHsv(image);

    BOOST_CHECK_EQUAL(hsv.size(), image.size());
    BOOST_CHECK_EQUAL(hsv.type(), image.type());
    BOOST_CHECK(hasMinMax(hsv, 0., 1.));

    Vec3i levels(3, 2, 2);
    Mat quantized = quantize(hsv, levels + Vec3i(0, 1, 1));
    BOOST_CHECK_EQUAL(quantized.size(), image.size());
    BOOST_CHECK_EQUAL(quantized.type(), image.type());

    if (argc > 2) {
        display(image);
        display(swapChannels(hsv));

        vector<Mat> planes;
        split(quantized, planes);
        planes[0] /= levels[0];
        planes[1] /= levels[1] + 1;
        planes[2] /= levels[2] + 1;
        merge(planes, quantized);
        display(swapChannels(quantized));

        print("Press a key to continue");
        waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(test_histogram) {
    Vec3i levels(3, 2, 2);
    bool normalize = argc > 2;

    HsvExtractor extractor(levels);

    Mat quantized;
    Mat histogram = extractor.extract(image, normalize, quantized);
    print(histogram);

    int numbins = (levels[0] * levels[1] * levels[2] + levels[2] + 1);
    BOOST_CHECK_EQUAL(Size(1, numbins), histogram.size()); // NOTE size is (cols, rows)
    if (normalize) BOOST_CHECK_CLOSE(1., sum(histogram)[0], 1e-5);

    BOOST_CHECK(not quantized.empty());

    Mat histImage = extractor.render(histogram);

    if (argc > 2) {
        imshow("hsv", histImage);
        imshow("quantized", quantized);

        print("Press a key to continue");
        waitKey(0);
    }
}

BOOST_AUTO_TEST_SUITE_END()

