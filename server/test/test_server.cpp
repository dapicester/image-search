/**
 * @file test_server.cpp
 * @brief Test case for image search server.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE server
#include <boost/test/unit_test.hpp>

#define _ELPP_THREAD_SAFE

#include "client.hpp"
#include "directories.h"
#include "protocol.hpp"
#include "server.hpp"

#include <vis/configuration.hpp>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

static const std::string HOST = "0.0.0.0";
static const std::string PORT = "4567";

struct InitLogging {
    InitLogging() {
        google::InitGoogleLogging("vis_server");
    }
    ~InitLogging() {
        google::ShutdownGoogleLogging();
    }
};

struct ServerConfig {
    ServerConfig() {
        config.categories.push_back({ "bag", DATA_DIR, {"color"} });
        config.categories.push_back({ "shoe", DATA_DIR, {"color","shape"} });
    }
    vis::config::Configuration config;
};

BOOST_FIXTURE_TEST_SUITE(test, InitLogging)

BOOST_AUTO_TEST_CASE(no_connect) {
    vis::Client client(HOST, PORT);
    BOOST_CHECK(not client.probe());
}

BOOST_FIXTURE_TEST_CASE(connect, ServerConfig) {
    vis::server::Server server(HOST, PORT, config);
    server.startAsync();
    {
        vis::Client client(HOST, PORT);
        BOOST_CHECK(client.probe());
    }
    server.stop();
}

BOOST_FIXTURE_TEST_CASE(request, ServerConfig) {
    vis::server::Server server(HOST, PORT, config);
    server.startAsync();

    const vis::Request requests[] = {
        { vis::RequestType::OFFLINE, "bag", vis::QueryType::COLOR, 20, 42, {} },
        { vis::RequestType::REALTIME, "shoe", vis::QueryType::SHAPE, 10, 0, std::vector<float>(300, 0.f) },
        // TODO upload
    };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::Request& request) {
        vis::Client client(HOST, PORT);
        BOOST_REQUIRE(client.probe());

        vis::Response response = client.sendRequest(request);

        BOOST_CHECK_EQUAL((int) vis::ResponseStatus::OK, (int) response.status);
        BOOST_CHECK_EQUAL(request.numResults, response.results.size()); // XXX actually should be CHECK_GE
        for (auto r : response.results) {
            static fs::path dir(DATA_DIR);
            BOOST_CHECK(fs::is_regular_file(dir/r.path));
        }
    });

    server.stop();
}

BOOST_FIXTURE_TEST_CASE(failure, ServerConfig) {
    vis::server::Server server(HOST, PORT, config);
    server.startAsync();

    const vis::Request failures[] = {
        { vis::RequestType::UPLOAD, "none", vis::QueryType::COLOR, 20 },
        { vis::RequestType::UPLOAD, "bag", static_cast<vis::QueryType>(5), 20 }
    };
    const vis::ResponseStatus expected[] = {
        vis::ResponseStatus::NO_CATEGORY,
        vis::ResponseStatus::NO_QUERY_TYPE
    };

    for (auto i = 0; i < 2; i++) {
        const vis::Request& request = failures[i];
        const vis::ResponseStatus& status = expected[i];

        vis::Client client(HOST, PORT);
        BOOST_REQUIRE(client.probe());

        vis::Response response = client.sendRequest(request);

        BOOST_CHECK_EQUAL((int) status, (int) response.status);
        BOOST_CHECK(response.results.empty());
    };

    server.stop();
}

BOOST_AUTO_TEST_SUITE_END();

