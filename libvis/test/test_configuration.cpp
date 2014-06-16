/**
 * @file test_configuration.cpp
 * @brief Tests for the configuration.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE configuration
#include <boost/test/unit_test.hpp>

#include "vis/configuration.hpp"
#include "directories.h"

#include <iostream>

#define argc boost::unit_test::framework::master_test_suite().argc
#define argv boost::unit_test::framework::master_test_suite().argv

using vis::config::Category;

BOOST_AUTO_TEST_CASE(load) {
    BOOST_REQUIRE_MESSAGE(argc == 2, "need to specify the YAML config file");

    vis::config::Configuration config = vis::config::loadConfiguration(argv[1]);

    BOOST_CHECK_EQUAL(config.categories.size(), 3);
    for (auto cat : config.categories) {
        BOOST_CHECK_EQUAL(cat.dir, DATA_DIR);
        BOOST_CHECK_GE(cat.type.size(), 1);
    }
}

BOOST_AUTO_TEST_CASE(dump) {
    vis::config::Configuration conf;
    conf.categories.push_back({ "bag", "path/to/dir", {"color"} });
    conf.categories.push_back({ "shoe", "path/to/dir", {"shape"} });
    conf.categories.push_back({ "woman_shoe", "path/to/dir", {"color", "shape"} });

    std::ostringstream ss;
    vis::config::dumpConfiguration(ss, conf);

    std::string yaml = ss.str();
    std::cout << yaml << std::endl;

    BOOST_CHECK(not yaml.empty());
}

