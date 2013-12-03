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

//#include <boost/scoped_ptr.hpp>
#include <random>

//typedef boost::scoped_ptr<vis::Callback> CallbackPtr;

BOOST_FIXTURE_TEST_CASE(test_load, test::Lena) {
    const std::string categories[] = { "bag", "shoe", "woman_shoe" };
    const vis::DescriptorsType types[] = { vis::HOG, vis::HSV, vis::HOG_HSV };

    for (const std::string& category : categories) {
        for (const vis::DescriptorsType type : types) {
            vis::ImageSearch imsearch(category, type);

            {
                // query with one of the indexed images id
                const unsigned id = rand() % 100;
                std::vector<size_t> results;
                imsearch.query(id, results);
                BOOST_REQUIRE_EQUAL(1, results.size());
                BOOST_REQUIRE_EQUAL(id, results[0]);
            }
            {
                // query with descriptors array
                arma::fvec descriptors = imsearch.extract(input);

                std::vector<size_t> results;
                imsearch.query(descriptors, results);
                BOOST_REQUIRE_EQUAL(1, results.size());
            }
            {
                // query with external image
                std::vector<size_t> results;
                imsearch.query(input, results);
                BOOST_REQUIRE_EQUAL(1, results.size());
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(test_save) {
    // save all components
}

BOOST_AUTO_TEST_CASE(test_build) {
    // build index/vocabulary
}

BOOST_AUTO_TEST_CASE(test_descriptors) {
    // compute descriptors
}

