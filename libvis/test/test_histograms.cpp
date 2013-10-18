/**
 * @file test_histograms.cpp
 * @brief Test for histogram computation
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE histograms
#include <boost/test/unit_test.hpp>

#include "extract.hpp"
#include "fixtures.hpp"
#include "vocabulary.hpp"
#include <boost/scoped_ptr.hpp>

namespace fs = boost::filesystem;

vis::Vocabulary* loadVocabulary() {
    static const fs::path vocabularyFile = "test_vocabulary.dat";
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(vocabularyFile), "Cannot find vocabulary file");

    vis::Vocabulary* vp = vis::Vocabulary::load(vocabularyFile);
    BOOST_CHECK(vp);

    return vp;
};

BOOST_FIXTURE_TEST_SUITE(histograms, test::ImageDir)

BOOST_AUTO_TEST_CASE(test_bow) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary( loadVocabulary() );

    cv::Mat descriptors;
    vis::HogBagOfWordsCallback cb(vocabulary.get());
    vis::extract(files, descriptors, cb, vis::GRAYSCALE);

    BOOST_CHECK_EQUAL(cv::Size(files.size(), vocabulary->getNumWords()), descriptors.size());
}

BOOST_AUTO_TEST_CASE(test_hsv) {
    cv::Mat histograms;
    vis::HsvHistogramsCallback cb;
    vis::extract(files, histograms, cb);

    BOOST_CHECK_EQUAL(cv::Size(files.size(), cb.getNumBins()), histograms.size());
}

BOOST_AUTO_TEST_CASE(test_bow_hsv) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary( loadVocabulary() );
#if 0
    // TODO this is the interface I want

    HogBagOfWordsCallback hog(vocabulary.get());
    HsvHistogramsCallback hsv;

    Extractor extractor;
    extractor.set(hog);
    extractor.set(hsv);

    cv::Mat descriptors;
    extractor.extract(files, descriptors);

    size_t descriptorSize = hsv.getNumBins() + vocabulary->getNumWords();
#else
    // XXX this is the quick'n dirty solution

    cv::Mat descriptors;
    vis::CompositeCallback cb(vocabulary.get());
    vis::extract(files, descriptors, cb);

    size_t descriptorsSize = cb.getNumBins() + vocabulary->getNumWords();
#endif
    BOOST_CHECK_EQUAL(cv::Size(files.size(), descriptorsSize), descriptors.size());
}

BOOST_AUTO_TEST_SUITE_END()

