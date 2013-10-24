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
#include <boost/scoped_ptr.hpp>

namespace fs = boost::filesystem;

vis::Vocabulary* loadVocabulary() {
    static const fs::path vocabularyFile = "test_vocabulary.dat";
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(vocabularyFile), "Cannot find vocabulary file");

    vis::Vocabulary* vp = vis::Vocabulary::load(vocabularyFile);
    BOOST_CHECK(vp);

    return vp;
};

#define USE_OLD_API 0

BOOST_FIXTURE_TEST_SUITE(test_descriptors, test::ImageDir)

BOOST_AUTO_TEST_CASE(hog) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary( loadVocabulary() );
    vis::HogBagOfWordsCallback cb(vocabulary.get());

    const cv::Size expectedSize(files.size(), vocabulary->getNumWords());
#if USE_OLD_API
    cv::Mat descriptors;
    vis::extract(files, descriptors, cb, vis::GRAYSCALE);

    BOOST_CHECK_EQUAL(expectedSize, descriptors.size());
#else
    vis::Descriptors descriptors;
    descriptors.compute("test", files, cb, vis::GRAYSCALE);

    BOOST_CHECK_EQUAL(expectedSize, descriptors.get().size());
    BOOST_CHECK_EQUAL(vis::HOG, descriptors.getType());
#endif
}

BOOST_AUTO_TEST_CASE(hsv) {
    vis::HsvHistogramsCallback cb;

    const cv::Size expectedSize(files.size(), cb.getNumBins());
#if USE_OLD_API
    cv::Mat histograms;
    vis::extract(files, histograms, cb);

    BOOST_CHECK_EQUAL(expectedSize, histograms.size());
#else
    vis::Descriptors histograms;
    histograms.compute("test", files, cb);

    BOOST_CHECK_EQUAL(expectedSize, histograms.get().size());
    BOOST_CHECK_EQUAL(vis::HSV, histograms.getType());
#endif
}

BOOST_AUTO_TEST_CASE(hog_hsv) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary( loadVocabulary() );

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

    const cv::Size expectedSize(files.size(), cb.getNumBins() + vocabulary->getNumWords());
#if USE_OLD_API
    cv::Mat descriptors;
    vis::extract(files, descriptors, cb);

    BOOST_CHECK_EQUAL(expectedSize, descriptors.size());
#else
    vis::Descriptors descriptors;
    descriptors.compute("test", files, cb);

    BOOST_CHECK_EQUAL(expectedSize, descriptors.get().size());
    BOOST_CHECK_EQUAL(vis::HOG_HSV, descriptors.getType());
#endif
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

