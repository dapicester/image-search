/**
 * @file test_descriptors.cpp
 * @brief Test for descriptors computation.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE descriptors
#include <boost/test/unit_test.hpp>

#include "callbacks.hpp"
#include "descriptors.hpp"
#include "fixtures.hpp"
#include "vocabulary.hpp"
#include "utils/matrix.hpp"
#include "utils/print.hpp"
#include "test_commons.hpp"
#include <boost/scoped_ptr.hpp>

BOOST_FIXTURE_TEST_SUITE(descriptors, test::ImageDir)

static const fs::path VOCABULARY_FILE = "test_vocabulary.dat";

typedef boost::scoped_ptr<vis::Vocabulary> VocabularyPtr;
typedef boost::scoped_ptr<vis::Descriptors> DescriptorsPtr;

void testSerialization(const fs::path& file, const vis::Descriptors& descriptors) {
    test::save(file, descriptors);
    DescriptorsPtr loaded(test::load<vis::Descriptors>(file));
    BOOST_CHECK_EQUAL(descriptors.getCategory(), loaded->getCategory());
    BOOST_CHECK(test::equals(descriptors.get(), loaded->get()));
    BOOST_CHECK_EQUAL(descriptors.getType(), loaded->getType());
}

BOOST_AUTO_TEST_CASE(hog) {
    VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));
    vis::HogBagOfWordsCallback cb(*vocabulary);

    vis::Descriptors descriptors;
    descriptors.compute("test", files, cb, vis::GRAYSCALE);
    const arma::fmat& data = descriptors.get();

    BOOST_CHECK_EQUAL(cb.length(),  data.n_rows);
    BOOST_CHECK_EQUAL(files.size(), data.n_cols);
    BOOST_CHECK_EQUAL(vis::HOG, descriptors.getType());

    testSerialization("test_descriptors_hog.dat", descriptors);
}

BOOST_AUTO_TEST_CASE(hsv) {
    vis::HsvHistogramsCallback cb;

    vis::Descriptors histograms;
    histograms.compute("test", files, cb);
    const arma::fmat& data = histograms.get();

    BOOST_CHECK_EQUAL(cb.length(),  data.n_rows);
    BOOST_CHECK_EQUAL(files.size(), data.n_cols);
    BOOST_CHECK_EQUAL(vis::HSV, histograms.getType());

    testSerialization("test_descriptors_hsv.dat", histograms);
}

BOOST_AUTO_TEST_CASE(hoghsv) {
    VocabularyPtr vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));

    /*
     * TODO this is the interface I want
     *
     * HogBagOfWordsCallback hog(vocabulary.get());
     * HsvHistogramsCallback hsv;
     *
     * Extractor extractor;
     * extractor.set(hog);
     * extractor.set(hsv);
     *
     * cv::Mat descriptors;
     * extractor.extract(files, descriptors);
     *
     * size_t descriptorSize = hsv.length() + hog.length();
     */

    vis::CompositeCallback cb(*vocabulary);

    vis::Descriptors descriptors;
    descriptors.compute("test", files, cb);
    const arma::fmat& data = descriptors.get();

    BOOST_CHECK_EQUAL(cb.length(),  data.n_rows);
    BOOST_CHECK_EQUAL(files.size(), data.n_cols);
    BOOST_CHECK_EQUAL(vis::HOG_HSV, descriptors.getType());

    testSerialization("test_descriptors_hoghsv.dat", descriptors);
}

BOOST_AUTO_TEST_SUITE_END()

