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

BOOST_AUTO_TEST_CASE(hog) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));
    vis::HogBagOfWordsCallback cb(vocabulary.get());

    const size_t expectedCols = files.size();
    const size_t expectedRows = vocabulary->getNumWords();

    vis::Descriptors descriptors;
    descriptors.compute("test", files, cb, vis::GRAYSCALE);
    const arma::fmat& data = descriptors.get();

    BOOST_CHECK_EQUAL(expectedRows, data.n_rows);
    BOOST_CHECK_EQUAL(expectedCols, data.n_cols);
    BOOST_CHECK_EQUAL(vis::HOG, descriptors.getType());
}

BOOST_AUTO_TEST_CASE(hsv) {
    vis::HsvHistogramsCallback cb;

    const size_t expectedCols = files.size();
    const size_t expectedRows = cb.getNumBins();

    vis::Descriptors histograms;
    histograms.compute("test", files, cb);
    const arma::fmat& data = histograms.get();

    BOOST_CHECK_EQUAL(expectedRows, data.n_rows);
    BOOST_CHECK_EQUAL(expectedCols, data.n_cols);
    BOOST_CHECK_EQUAL(vis::HSV, histograms.getType());
}

BOOST_AUTO_TEST_CASE(hog_hsv) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary(test::load<vis::Vocabulary>(VOCABULARY_FILE));

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
     * size_t descriptorSize = hsv.getNumBins() + vocabulary->getNumWords();
     */

    vis::CompositeCallback cb(vocabulary.get());

    const size_t expectedCols = files.size();
    const size_t expectedRows = cb.getNumBins() + vocabulary->getNumWords();

    vis::Descriptors descriptors;
    descriptors.compute("test", files, cb);
    const arma::fmat& data = descriptors.get();

    BOOST_CHECK_EQUAL(expectedRows, data.n_rows);
    BOOST_CHECK_EQUAL(expectedCols, data.n_cols);
    BOOST_CHECK_EQUAL(vis::HOG_HSV, descriptors.getType());
}

BOOST_AUTO_TEST_CASE(serialization) {
    vis::HsvHistogramsCallback cb;
    fs::path file("test_descriptors.dat");

    vis::Descriptors histograms;
    histograms.compute("test", files, cb);
    histograms.save(file);

    vis::Descriptors* loaded = vis::Descriptors::load(file);
    BOOST_CHECK_EQUAL(histograms.getCategory(), loaded->getCategory());
    BOOST_CHECK(test::equals(histograms.get(), loaded->get()));
    BOOST_CHECK_EQUAL(histograms.getType(), loaded->getType());
}

BOOST_AUTO_TEST_SUITE_END()

