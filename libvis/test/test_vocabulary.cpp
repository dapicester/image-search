/**
 * @file test_vocabulary.cpp
 * @brief Test for visual words vocabulary
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE vocabulary
#include <boost/test/unit_test.hpp>

#include "vocabulary.hpp"
#include "utils/filesystem.hpp"
#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

namespace fs = boost::filesystem;

using boost::scoped_ptr;
using namespace std;
using namespace vis;

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

BOOST_AUTO_TEST_CASE(test_vocabulary) {
    BOOST_REQUIRE_MESSAGE(argc == 2, "Need to specify the data dir");
    const fs::path dataDir = argv[1];

    BOOST_REQUIRE_MESSAGE(fs::is_directory(dataDir), "invalid data dir");
    const fs::path imageDir = dataDir / "test";

    vector<fs::path> files = getImageFiles(imageDir);

    // arrange as matlab
    std::reverse(files.begin(), files.end());
    std::sort(files.begin(), files.end());

    // compute vocabulary
    scoped_ptr<Vocabulary> vocabulary(Vocabulary::fromImageList("test", files));
    BOOST_CHECK(vocabulary.get());

    // check it's the same as matlab (see test_vocabulary.mat)
    const VlKDForest* forest = vocabulary->getKDTree()->getForest();
    BOOST_CHECK_EQUAL(300, vocabulary->getNumWords());
    BOOST_CHECK_EQUAL(cv::Size(300,28), vocabulary->getWords().size());
    BOOST_CHECK_EQUAL(1, vl_kdforest_get_num_trees(forest));
    BOOST_CHECK_EQUAL(9, vl_kdforest_get_depth_of_tree(forest, 0));
    BOOST_CHECK_EQUAL(599, vl_kdforest_get_num_nodes_of_tree(forest, 0));

    // save
    const fs::path vocabularyFile = "test_vocabulary.dat";
    if (fs::exists(vocabularyFile)) fs::remove(vocabularyFile);
    BOOST_CHECK(not fs::exists(vocabularyFile));

    vocabulary->save(vocabularyFile);
    BOOST_CHECK(fs::is_regular_file(vocabularyFile));
}

BOOST_AUTO_TEST_CASE(test_serialization) {
    const fs::path vocabularyFile = "test_vocabulary.dat";
    BOOST_REQUIRE_MESSAGE(fs::is_regular_file(vocabularyFile), "Cannot find vocabulary file");

    scoped_ptr<Vocabulary> vocabulary(Vocabulary::load(vocabularyFile));
    BOOST_CHECK(vocabulary.get());
    // check it's the same
    const VlKDForest* forest = vocabulary->getKDTree()->getForest();
    BOOST_CHECK_EQUAL(300, vocabulary->getNumWords());
    BOOST_CHECK_EQUAL(cv::Size(300,28), vocabulary->getWords().size());
    BOOST_CHECK_EQUAL(1, vl_kdforest_get_num_trees(forest));
    BOOST_CHECK_EQUAL(9, vl_kdforest_get_depth_of_tree(forest, 0));
    BOOST_CHECK_EQUAL(599, vl_kdforest_get_num_nodes_of_tree(forest, 0));
}

