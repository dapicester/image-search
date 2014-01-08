/**
 * @file test_server.cpp
 * @brief Test case for image search server.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE server
#include <boost/test/unit_test.hpp>

#define _ELPP_THREAD_SAFE

#include "client.hpp"
#include "configuration.hpp"
#include "directories.h"
#include "logging.hpp"
#include "protocol.hpp"
#include "server.hpp"

#include <boost/serialization/export.hpp>

_INITIALIZE_EASYLOGGINGPP

BOOST_CLASS_EXPORT(vis::OfflineRequest);
BOOST_CLASS_EXPORT(vis::RealtimeRequest);
BOOST_CLASS_EXPORT(vis::UploadRequest);

static const std::string HOST = "0.0.0.0";
static const std::string PORT = "4567";

struct InitLogging {
    InitLogging() {
        vis::logging::registerLoggers({ "server", "client", "connection", "handler", "manager" });
    }
};

struct ServerConfig {
    ServerConfig() {
        config.categories.push_back({ "bag", DATA_DIR, "color" });
        config.categories.push_back({ "shoe", DATA_DIR, "shape" });
    }
    vis::server::Configuration config;
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

static const vis::OfflineRequest offline(
        vis::RequestType::OFFLINE, "bag", 'c', 20,
        42);
static const vis::RealtimeRequest realtime(
        vis::RequestType::REALTIME, "shoe", 's', 10,
        std::vector<float>(300, 0.f));
static const vis::UploadRequest upload(vis::RequestType::UPLOAD,
        "none", 'c', 20,
        nullptr /* TODO image data*/);

BOOST_FIXTURE_TEST_CASE(request, ServerConfig) {
    vis::server::Server server(HOST, PORT, config);
    server.startAsync();

    const vis::BaseRequest* requests[] = { &offline, &realtime };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::BaseRequest* request) {
        vis::Client client(HOST, PORT);
        BOOST_REQUIRE(client.probe());

        vis::Response response = client.sendRequest(request);

        const std::vector<vis::id_type>& matches = response.results;
        BOOST_CHECK_EQUAL((int) vis::ResponseStatus::OK, (int) response.status);
        BOOST_CHECK(response.message.empty());
        BOOST_CHECK_EQUAL(request->numResults, matches.size()); // XXX actually should be CHECK_GE
    });

    const vis::BaseRequest* failures[] = { &upload };
    std::for_each(std::begin(failures), std::end(failures), [](const vis::BaseRequest* request) {
        vis::Client client(HOST, PORT);
        BOOST_REQUIRE(client.probe());

        vis::Response response = client.sendRequest(request);

        const std::vector<vis::id_type>& matches = response.results;
        BOOST_CHECK_EQUAL((int) vis::ResponseStatus::ERROR, (int) response.status);
        BOOST_CHECK(not response.message.empty());
        BOOST_CHECK(matches.empty());
    });



    server.stop();
}

BOOST_AUTO_TEST_SUITE_END();

