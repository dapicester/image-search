/**
 * @file test_hog_bow.cpp
 * @brief Test for BOW descriptors using HOG.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hog_bow
#include <boost/test/unit_test.hpp>

#include "bow.hpp"
#include "hog.hpp"
#include "vocabulary.hpp"
#include "fixtures.hpp"
//#include "utils/data.hpp"
//#include "utils/matrix.hpp"
//#include "utils/print.hpp"
//#include "utils/conversions.hpp"
#include "test_commons.hpp"
#include <boost/scoped_ptr.hpp>
//#include <opencv2/highgui/highgui.hpp>

#define argc boost::unit_test::framework::master_test_suite().argc

static const fs::path VOCABULARY_FILE = "test_vocabulary.dat";

typedef boost::scoped_ptr<vis::Vocabulary> VocabularyPtr;
typedef vis::BagOfWords<vis::HogExtractor> BowHog;

BOOST_FIXTURE_TEST_CASE(compute, test::Lena) {
    VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));
    BowHog extractor(*vocabulary);
}

BOOST_AUTO_TEST_CASE(bow_values) {
#if 0
    vis::Vocabulary* vocabulary = NULL;
    vis::BowExtractor extractor(vocabulary);

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
#endif
}

