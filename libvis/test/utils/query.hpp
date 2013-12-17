/**
 * @file utils/query.hpp
 * @brief Utils for querying ImageSearch.
 * @author Paolo D'Apice
 */

#include "vis/imsearch.hpp"

#include <boost/test/unit_test.hpp>

namespace test {

void queryById(const vis::ImageSearch& imsearch, const unsigned id) {
    std::vector<size_t> results;
    imsearch.query(id, results);
    BOOST_REQUIRE_EQUAL( 1, results.size());
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

} // namespace test

