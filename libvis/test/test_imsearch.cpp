/**
 * @file test_imsearch.cpp
 * @brief Test for image search façade.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE imsearch
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "test_commons.hpp"

#include "vis/imsearch.hpp"

#include <random>

void queryById(const vis::ImageSearch& imsearch, const unsigned id) {
    std::vector<size_t> results;
    imsearch.query(id, results);
    BOOST_REQUIRE_EQUAL(1, results.size());
    BOOST_REQUIRE_EQUAL(id, results[0]);
}

void queryByDescriptors(const vis::ImageSearch& imsearch, const cv::Mat image) {
    arma::fvec descriptors = imsearch.extract(image);

    std::vector<size_t> results;
    imsearch.query(descriptors, results);
    BOOST_REQUIRE_EQUAL(1, results.size());
}

void queryByImage(const vis::ImageSearch& imsearch, const cv::Mat image) {
    std::vector<size_t> results;
    imsearch.query(image, results);
    BOOST_REQUIRE_EQUAL(1, results.size());
}

/// Load image search from file.
BOOST_FIXTURE_TEST_CASE(test_load, test::Lena) {
    const std::string categories[] = { "bag", "shoe", "woman_shoe" };
    const vis::DescriptorsType types[] = { vis::HOG, vis::HSV, vis::HOG_HSV };

    for (const std::string& category : categories) {
        for (const vis::DescriptorsType type : types) {
            vis::ImageSearch imsearch(category, type, DATA_DIR);
            imsearch.load();

            // query with one of the indexed images id
            queryById(imsearch, rand() % 100);

            // query with descriptors array
            queryByDescriptors(imsearch, input);

            // query with external image
            queryByImage(imsearch, input);
        }
    }
}

/// Build index/vocabulary and save to files.
BOOST_AUTO_TEST_CASE(test_build) {
    const std::string categories[] = { "bag", "shoe", "woman_shoe" };
    const vis::DescriptorsType types[] = { vis::HOG, vis::HSV, vis::HOG_HSV };

    for (const std::string& category : categories) {
        for (const vis::DescriptorsType type : types) {
            vis::ImageSearch imsearch(category, type, DATA_DIR);
            imsearch.build();
            imsearch.save();
        }
    }

}

