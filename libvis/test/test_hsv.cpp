/**
 * @file test_hsv.cpp
 * @brief Test for HSV color histogram
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hsv
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "standardize.hpp"
#include "hsv.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include <opencv2/highgui/highgui.hpp>

using namespace vis;
using namespace cv;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

#define display(TITLE, IMAGE) \
            namedWindow((TITLE), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED); \
            imshow((TITLE),(IMAGE));

Mat swapChannels(const Mat& in) {
    Mat out(in.size(), in.type());
    const static int fromto[] = { 0,2,  1,1,  2,0 };
    mixChannels(&in, 1, &out, 1, fromto, 3);
    return out;
}

BOOST_FIXTURE_TEST_CASE(test_functions, Peppers) {
    Mat hsv = toHsv(image);

    BOOST_CHECK_EQUAL(hsv.size(), image.size());
    BOOST_CHECK_EQUAL(hsv.type(), image.type());
    BOOST_CHECK(hasMinMax(hsv, 0., 1.));

    Vec3i levels(3, 2+1, 2+1);
    Mat quantized = quantize(hsv, levels);
    BOOST_CHECK_EQUAL(quantized.size(), image.size());
    BOOST_CHECK_EQUAL(quantized.type(), image.type());

    vector<Mat> planes;
    split(quantized, planes);
    BOOST_CHECK(hasMinMax(planes[0], 1, levels[0]));
    BOOST_CHECK(hasMinMax(planes[1], 1, levels[1]));
    BOOST_CHECK(hasMinMax(planes[2], 1, levels[2]));

    if (argc > 1) {
        display("image", image);
        display("hsv", swapChannels(hsv)); // HSV channels are interpreted as BGR

        planes[0] /= levels[0];
        planes[1] /= levels[1];
        planes[2] /= levels[2];
        merge(planes, quantized);
        display("quantized", swapChannels(quantized)); // idem

        Mat bgr = toBgr(quantized);
        display("rgb", bgr);

        print("Press a key to continue");
        waitKey(0);
    }
}

BOOST_FIXTURE_TEST_CASE(test_histogram, Peppers) {
    Vec3i levels(3,2,2);
    bool normalize = argc > 1;

    HsvExtractor extractor(levels);

    Mat quantized;
    Mat histogram = extractor.extract(image, normalize, quantized);

    print(histogram);

    int numbins = extractor.getNumBins();
    BOOST_CHECK_EQUAL(Size(1, numbins), histogram.size()); // NOTE size is (cols, rows)
    if (normalize) BOOST_CHECK_CLOSE(1., sum(histogram)[0], 1e-5);

    BOOST_CHECK(not quantized.empty());

    Mat histImage = extractor.render(histogram);

    if (argc > 1) {
        imshow("hsv", histImage);
        imshow("quantized", quantized);

        print("Press a key to continue");
        waitKey(0);
    }
}

