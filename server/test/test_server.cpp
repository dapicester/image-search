/**
 * @file test_server.cpp
 * @brief Test case for image search server.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE server
#include <boost/test/unit_test.hpp>

#include "client.hpp"
#include "server.hpp"

static const short PORT = 4567;

BOOST_AUTO_TEST_CASE(test_no_connect) {
    vis::Client client("localhost", PORT);
    BOOST_CHECK(not client.probe());
}

BOOST_AUTO_TEST_CASE(test_connect) {
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

BOOST_AUTO_TEST_CASE(test_request) {
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

    std::this_thread::sleep_for(std::chrono::seconds(1)); // wait for stdout
}

