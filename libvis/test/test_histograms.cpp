/**
 * @file test_histograms.cpp
 * @brief Test for histogram computation
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE histograms
#include <boost/test/unit_test.hpp>

#include "histograms.hpp"
#include "hog.hpp"
#include "standardize.hpp"
#include "utils/filesystem.hpp"
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

    // compute hog
    HogExtractor hog;
    Mat descriptors;

    // TODO openmp parfor
    // TODO refactor this and vocabulary.cpp
    size_t i = 0, numImages = files.size();
    for (vector<fs::path>::const_iterator it = files.begin(); it != files.end(); ++it) {
        const string& name = it->string();
        printf("  Extracting features from %s (%lu/%lu)\n", name.c_str(), i+1, numImages);

        Mat input = cv::imread(name, CV_LOAD_IMAGE_GRAYSCALE); // hog needs only grayscale
        Mat image; // TODO change standardize signature to allow return?
        standardizeImage(input, image);

        Mat d = hog.extract(image).toMat();
        Mat hist = bowHistogram(d, *vocabulary).t();

        descriptors.push_back(hist);
        ++i;
    }
    descriptors = descriptors.t();

    BOOST_CHECK_EQUAL(cv::Size(numImages, vocabulary->getNumWords()), descriptors.size());
}

