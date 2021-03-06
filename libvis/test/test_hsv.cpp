/**
 * @file test_hsv.cpp
 * @brief Test for HSV color histogram
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hsv
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"

#include "vis/hsv.hpp"

#include <opencv2/highgui/highgui.hpp>

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

#define display(TITLE, IMAGE) \
    do { \
        cv::namedWindow((TITLE), CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED); \
        cv::imshow((TITLE),(IMAGE)); \
    } while(0)

BOOST_FIXTURE_TEST_CASE(test_histogram, test::Peppers) {
    vis::hsv::Levels levels = {3,2,2};
    vis::HsvExtractor extractor(levels);

    cv::Mat quantized;
    arma::fvec histogram = extractor.extract(image, true, quantized);
    float sum = arma::sum(histogram);

    printmat(histogram);
    printvar(sum);

    int numbins = extractor.getNumBins();
    BOOST_CHECK_EQUAL(histogram.size(), numbins);
    BOOST_CHECK_CLOSE_FRACTION(sum, 1., 0.1);

    BOOST_CHECK(not quantized.empty());

    cv::Mat histImage = extractor.render(histogram);

    if (argc > 1) {
        display("histogram", histImage);
        display("image", image);
        display("quantized", quantized);

        println("Press a key to continue");
        cv::waitKey(0);
    }
}

