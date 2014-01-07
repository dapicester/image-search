/**
 * @file test_configuration.cpp
 * @brief Tests for the server configuration.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE configuration
#include <boost/test/unit_test.hpp>

#include "configuration.hpp"
#include "directories.h"

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

using vis::server::Category;

BOOST_AUTO_TEST_CASE(load) {
    BOOST_REQUIRE_MESSAGE(argc == 2, "need to specify the YAML config file");

    vis::server::Configuration config = vis::server::loadConfiguration(argv[1]);

    BOOST_CHECK_EQUAL(config.categories.size(), 2);
    for (auto cat : config.categories) {
        BOOST_CHECK_EQUAL(cat.dir, DATA_DIR);
    }
}

BOOST_AUTO_TEST_CASE(dump) {
    vis::server::Configuration conf;
    conf.categories.push_back({ "bag", "path/to/dir", "color" });
    conf.categories.push_back({ "shoe", "path/to/dir", "shape" });

    std::ostringstream ss;
    vis::server::dumpConfiguration(ss, conf);

    std::string yaml = ss.str();
    std::cout << yaml << std::endl;

    BOOST_CHECK(not yaml.empty());
}

