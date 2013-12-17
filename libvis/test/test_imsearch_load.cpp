/**
 * @file test_imsearch_load.cpp
 * @brief Test for image search façade (load from file).
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE imsearch
#include <boost/test/unit_test.hpp>

#include "fixtures.hpp"
#include "test_commons.hpp"
#include "utils/query.hpp"

#include <random>

/// Load image search from file.
BOOST_FIXTURE_TEST_CASE(test_load, test::Lena) {
    const std::string categories[] = { "bag", "shoe", "woman_shoe" };
    const vis::DescriptorsType types[] = { vis::HOG, vis::HSV, vis::HOG_HSV };

    for (const std::string& category : categories) {
        for (const vis::DescriptorsType type : types) {
            vis::ImageSearch imsearch(category, type, DATA_DIR);
            imsearch.load();

            // query with one of the indexed images id
            test::queryById(imsearch, rand() % 100);

            // query with descriptors array
            test::queryByDescriptors(imsearch, input);

            // query with external image
            test::queryByImage(imsearch, input);
        }
    }
}

