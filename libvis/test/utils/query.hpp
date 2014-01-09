/**
 * @file utils/query.hpp
 * @brief Utils for querying ImageSearch.
 * @author Paolo D'Apice
 */

#include "directories.h"
#include "vis/imsearch.hpp"

#include <boost/test/unit_test.hpp>

namespace test {

namespace fs = boost::filesystem;

typedef std::vector<fs::path> path_vector;

auto is_file = [](const fs::path& path) {
    return fs::is_regular_file(path);
};

static const fs::path dataDir = DATA_DIR;

void checkFiles(const path_vector& files, bool absolute) {
    for (auto file : files) {
        if (absolute) {
            BOOST_REQUIRE(is_file(file));
        } else {
            BOOST_REQUIRE(not is_file(file));
            BOOST_REQUIRE(is_file(dataDir / file));
        }
    }
}

void queryById(const vis::ImageSearch& imsearch, const unsigned id) {
    std::vector<size_t> results;
    imsearch.query(id, results);
    BOOST_REQUIRE_EQUAL( 1, results.size());
    BOOST_REQUIRE_EQUAL(id, results[0]);

    bool absolute = true;

    path_vector files = imsearch.get(results, absolute);
    BOOST_REQUIRE_EQUAL(1, results.size());
    checkFiles(files, absolute);
}

void queryByDescriptors(const vis::ImageSearch& imsearch, const cv::Mat image) {
    arma::fvec descriptors = imsearch.extract(image);

    std::vector<size_t> results;
    imsearch.query(descriptors, results);
    BOOST_REQUIRE_EQUAL(1, results.size());

    bool absolute = true;

    path_vector files = imsearch.get(results, absolute);
    BOOST_REQUIRE_EQUAL(1, results.size());
    checkFiles(files, absolute);
}

void queryByImage(const vis::ImageSearch& imsearch, const cv::Mat image) {
    std::vector<size_t> results;
    imsearch.query(image, results);
    BOOST_REQUIRE_EQUAL(1, results.size());

    bool absolute = false;

    path_vector files = imsearch.get(results, absolute);
    BOOST_REQUIRE_EQUAL(1, results.size());
    checkFiles(files, absolute);
}

} // namespace test

