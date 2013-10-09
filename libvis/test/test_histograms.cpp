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

BOOST_AUTO_TEST_CASE(test_bow) {
    BOOST_REQUIRE_MESSAGE(argc == 2, "Need to specify the root dir");
    const fs::path rootDir = argv[1];

    BOOST_REQUIRE_MESSAGE(fs::is_directory(rootDir), "invalid root dir");
    const fs::path imageDir = rootDir / "images";

    vector<fs::path> files = getImageFiles(imageDir);

    // arrange as matlab
    std::reverse(files.begin(), files.end());
    std::sort(files.begin(), files.end());

    // load vocabulary
    const fs::path vocabularyFile = "test_vocabulary.dat";
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(vocabularyFile), "Cannot find vocabulary file");

    scoped_ptr<Vocabulary> vocabulary(Vocabulary::load(vocabularyFile));
    BOOST_CHECK(vocabulary.get());

    // compute bow
    Mat descriptors;
    HogBagOfWordsCallback cb(vocabulary.get());
    extract(files, descriptors, cb);

    BOOST_CHECK_EQUAL(cv::Size(files.size(), vocabulary->getNumWords()), descriptors.size());
}

