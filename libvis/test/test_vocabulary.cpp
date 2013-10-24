/**
 * @file test_vocabulary.cpp
 * @brief Test for visual words vocabulary
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE vocabulary
#include <boost/test/unit_test.hpp>

#include "vocabulary.hpp"
#include "fixtures.hpp"
#include <boost/scoped_ptr.hpp>

namespace fs = boost::filesystem;

BOOST_FIXTURE_TEST_CASE(test_vocabulary, test::ImageDir) {
    // compute vocabulary
    boost::scoped_ptr<vis::Vocabulary> vocabulary(vis::Vocabulary::fromImageList("test", files));
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

    boost::scoped_ptr<vis::Vocabulary> vocabulary(vis::Vocabulary::load(vocabularyFile));
    BOOST_CHECK(vocabulary.get());
    // check it's the same
    const VlKDForest* forest = vocabulary->getKDTree()->getForest();
    BOOST_CHECK_EQUAL("test", vocabulary->getCategory());
    BOOST_CHECK_EQUAL(300, vocabulary->getNumWords());
    BOOST_CHECK_EQUAL(cv::Size(300,28), vocabulary->getWords().size());
    BOOST_CHECK_EQUAL(1, vl_kdforest_get_num_trees(forest));
    BOOST_CHECK_EQUAL(9, vl_kdforest_get_depth_of_tree(forest, 0));
    BOOST_CHECK_EQUAL(599, vl_kdforest_get_num_nodes_of_tree(forest, 0));
}

