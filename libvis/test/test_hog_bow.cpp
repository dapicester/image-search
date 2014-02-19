/**
 * @file test_hog_bow.cpp
 * @brief Test for BOW descriptors using HOG.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE hog_bow
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "utils/print.hpp"
#include "test_commons.hpp"

#include "vis/bow.hpp"
#include "vis/hog.hpp"
#include "vis/vocabulary.hpp"

#include <boost/scoped_ptr.hpp>

static const fs::path VOCABULARY_FILE = "test_vocabulary.dat";

typedef boost::scoped_ptr<vis::Vocabulary> VocabularyPtr;
typedef vis::BagOfWords<vis::HogExtractor> BowHog;

BOOST_FIXTURE_TEST_CASE(test_bow, test::Peppers) {
    VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));
    BowHog extractor(*vocabulary);

    arma::fvec descriptors = extractor.extract(image);
    float sum = arma::sum(descriptors);

    printmat(descriptors);
    printvar(sum);

    BOOST_CHECK_EQUAL(extractor.numWords(), descriptors.size());
    BOOST_CHECK_CLOSE_FRACTION(sum, 1., 0.1);
}

