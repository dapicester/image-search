/**
 * @file test_server.cpp
 * @brief Test case for image search server.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE server
#include <boost/test/unit_test.hpp>

#include "client.hpp"
#include "logging.hpp"
#include "server.hpp"

_INITIALIZE_EASYLOGGINGPP

static const short PORT = 4567;

struct InitLogging {
    InitLogging() {
        vis::registerLoggers({ "server", "client", "connection" });
    }
};

BOOST_FIXTURE_TEST_SUITE(test, InitLogging)

BOOST_AUTO_TEST_CASE(no_connect) {
    vis::Client client("localhost", PORT);
    BOOST_CHECK(not client.probe());
}

BOOST_AUTO_TEST_CASE(connect) {
    vis::Server server(PORT);
    server.start();
    BOOST_REQUIRE(server.isRunning());
    {
        vis::Client client("localhost", PORT);
        BOOST_CHECK(client.probe());
    }
    server.stop();
    BOOST_REQUIRE(not server.isRunning());
}

BOOST_AUTO_TEST_CASE(request) {
    vis::Server server(PORT);
    server.start();
    BOOST_REQUIRE(server.isRunning());
    for (int i = 1; i < 3; i++) {
        vis::Client client("localhost", PORT);
        BOOST_REQUIRE(client.probe());

        std::string response = client.sendRequest("echo");
        BOOST_CHECK_EQUAL("echo", response);
    }
    server.stop();
    BOOST_REQUIRE(not server.isRunning());
}

BOOST_AUTO_TEST_SUITE_END();

