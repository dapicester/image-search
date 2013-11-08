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
#include "utils/conversions.hpp"
#include <opencv2/highgui/highgui.hpp>

#define argc boost::unit_test::framework::master_test_suite().argc

BOOST_FIXTURE_TEST_CASE(compute_hog, test::Lena) {
    vis::HogExtractor extractor;

    arma::fmat descriptors = extractor.extract(image);
    BOOST_CHECK_EQUAL(3*8+4, descriptors.n_rows);
    BOOST_CHECK_EQUAL(16*16, descriptors.n_cols);

    if (argc > 1) {
        cv::imshow("image", input);

        cv::Mat hogImage = extractor.render(descriptors);
        cv::imshow("hog", hogImage);

        println("Press a key to continue");
        cv::waitKey(0);
    }
}

BOOST_AUTO_TEST_CASE(hog_values) {
    vis::HogExtractor extractor;

    arma::fmat hog = extractor.extract(cv::Mat::eye(16, 16, CV_32F));
    BOOST_CHECK_EQUAL(28, hog.n_rows);
    BOOST_CHECK_EQUAL(4, hog.n_cols);

    printmat(hog);

    // from Matlab
    cv::Mat temp = (cv::Mat_<float>(28,4) <<
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
    arma::fmat expected = vis::cv2arma<float>(temp);

    printmat(expected);

    BOOST_REQUIRE_EQUAL(expected.size(), hog.size());
    BOOST_REQUIRE_EQUAL(expected.n_rows, hog.n_rows);
    BOOST_REQUIRE_EQUAL(expected.n_cols, hog.n_cols);

    for (int i = 0; i < expected.size() ; i++)
        BOOST_REQUIRE_CLOSE_FRACTION(expected(i), hog.at(i), 1e-6);
    //BOOST_REQUIRE(test::equals(expected, hog));

}

