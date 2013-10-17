/**
 * @file test_histograms.cpp
 * @brief Test for histogram computation
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE histograms
#include <boost/test/unit_test.hpp>

#include "extract.hpp"
#include "utils/filesystem.hpp"
#include "vocabulary.hpp"
#include <boost/scoped_ptr.hpp>

namespace fs = boost::filesystem;

using boost::scoped_ptr;
using cv::Mat;
using std::vector;
using std::string;
using namespace vis;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

struct ImageDir {
    ImageDir() {
        BOOST_REQUIRE_MESSAGE(argc == 2, "Need to specify the root dir");
        const fs::path rootDir = argv[1];

        BOOST_REQUIRE_MESSAGE(fs::is_directory(rootDir), "invalid root dir");
        const fs::path imageDir = rootDir / "images";

        files = getImageFiles(imageDir);

        // arrange as matlab
        std::reverse(files.begin(), files.end());
        std::sort(files.begin(), files.end());
    }
    vector<fs::path> files;
};

Vocabulary* loadVocabulary() {
    static const fs::path vocabularyFile = "test_vocabulary.dat";
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(vocabularyFile), "Cannot find vocabulary file");

    Vocabulary* vp = Vocabulary::load(vocabularyFile);
    BOOST_CHECK(vp);

    return vp;
};

BOOST_FIXTURE_TEST_SUITE(histograms, ImageDir)

BOOST_AUTO_TEST_CASE(test_bow) {
    scoped_ptr<Vocabulary> vocabulary( loadVocabulary() );

    Mat descriptors;
    HogBagOfWordsCallback cb(vocabulary.get());
    extract(files, descriptors, cb, GRAYSCALE);

    BOOST_CHECK_EQUAL(cv::Size(files.size(), vocabulary->getNumWords()), descriptors.size());
}

BOOST_AUTO_TEST_CASE(test_hsv) {
    Mat histograms;
    HsvHistogramsCallback cb;
    extract(files, histograms, cb);

    BOOST_CHECK_EQUAL(cv::Size(files.size(), cb.getNumBins()), histograms.size());
}

BOOST_AUTO_TEST_CASE(test_bow_hsv) {
    scoped_ptr<Vocabulary> vocabulary( loadVocabulary() );
#if 0
    // TODO this is the interface I want

    HogBagOfWordsCallback hog(vocabulary.get());
    HsvHistogramsCallback hsv;

    Extractor extractor;
    extractor.set(hog);
    extractor.set(hsv);

    Mat descriptors;
    extractor.extract(files, descriptors);

    size_t descriptorSize = hsv.getNumBins() + vocabulary->getNumWords();
#else
    // XXX this is the quick'n dirty solution

    Mat descriptors;
    CompositeCallback cb(vocabulary.get());
    extract(files, descriptors, cb);

    size_t descriptorsSize = cb.getNumBins() + vocabulary->getNumWords();
#endif
    BOOST_CHECK_EQUAL(cv::Size(files.size(), descriptorsSize), descriptors.size());
}

BOOST_AUTO_TEST_SUITE_END()

