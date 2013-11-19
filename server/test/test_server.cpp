/**
 * @file test_server.cpp
 * @brief Test case for image search server.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE server
#include <boost/test/unit_test.hpp>

#define _ELPP_THREAD_SAFE

#include "client.hpp"
#include "logging.hpp"
#include "protocol.hpp"
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

const vis::Request offline() {
    vis::Request r;
    r.requestType = 'o';
    r.category = "bag";
    r.queryType = 'c';
    r.numResults = 20;
    r.id = 42;
    return r;
}
//static const vis::Request realtime = { 'r', "bag", 'c', 20, 166, std::vector<float>(166, 0.f) };
//static const vis::Request upload   = { 'u', "bag", 'c', 20, 255, [>image data<] };

BOOST_AUTO_TEST_CASE(request) {
    vis::Server server(PORT);
    server.start();
    BOOST_REQUIRE(server.isRunning());

    vis::Request requests[] = { offline() };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::Request& request) {
        vis::Client client("localhost", PORT);
        BOOST_REQUIRE(client.probe());

        vis::Response response = client.sendRequest(request);
        BOOST_CHECK(response.results.empty()); // FIXME complete
    });
    server.stop();
    BOOST_REQUIRE(not server.isRunning());
}

BOOST_AUTO_TEST_SUITE_END();

