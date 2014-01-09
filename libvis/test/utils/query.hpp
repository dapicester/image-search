/**
 * @file utils/query.hpp
 * @brief Utils for querying ImageSearch.
 * @author Paolo D'Apice
 */

#include "vis/imsearch.hpp"

#include <boost/test/unit_test.hpp>

namespace test {

namespace fs = boost::filesystem;

typedef std::vector<fs::path> path_vector;

auto is_file = [](const fs::path& path) { return fs::is_regular_file(path); };

void queryById(const vis::ImageSearch& imsearch, const unsigned id) {
    std::vector<size_t> results;
    imsearch.query(id, results);
    BOOST_REQUIRE_EQUAL( 1, results.size());
    BOOST_REQUIRE_EQUAL(id, results[0]);

    path_vector files = imsearch.getImages(results);
    BOOST_REQUIRE_EQUAL(1, results.size());
    BOOST_REQUIRE(is_file(files[0]));
}

void queryByDescriptors(const vis::ImageSearch& imsearch, const cv::Mat image) {
    arma::fvec descriptors = imsearch.extract(image);

    std::vector<size_t> results;
    imsearch.query(descriptors, results);
    BOOST_REQUIRE_EQUAL(1, results.size());

    path_vector files = imsearch.getImages(results);
    BOOST_REQUIRE_EQUAL(1, results.size());
    BOOST_REQUIRE(is_file(files[0]));
}

void queryByImage(const vis::ImageSearch& imsearch, const cv::Mat image) {
    std::vector<size_t> results;
    imsearch.query(image, results);
    BOOST_REQUIRE_EQUAL(1, results.size());

    path_vector files = imsearch.getImages(results);
    BOOST_REQUIRE_EQUAL(1, results.size());
    BOOST_REQUIRE(is_file(files[0]));
}

} // namespace test

