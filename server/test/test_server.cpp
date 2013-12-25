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

#include <boost/serialization/export.hpp>

_INITIALIZE_EASYLOGGINGPP

BOOST_CLASS_EXPORT(vis::OfflineRequest);
BOOST_CLASS_EXPORT(vis::RealtimeRequest);
BOOST_CLASS_EXPORT(vis::UploadRequest);

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
    vis::Server server("localhost", "4567");
    server.startAsync();
    {
        vis::Client client("localhost", PORT);
        BOOST_CHECK(client.probe());
    }
    server.stop();
}

static const vis::OfflineRequest offline(vis::RequestType::OFFLINE, "bag", 'c', 20, 42);
static const vis::RealtimeRequest realtime(vis::RequestType::REALTIME, "bag", 'c', 20, std::vector<float>(166, 0.f));
static const vis::UploadRequest upload( vis::RequestType::UPLOAD, "bag", 'c', 20, nullptr /*image data*/);

BOOST_AUTO_TEST_CASE(request) {
    vis::Server server("localhost", "4567");
    server.startAsync();

    const vis::BaseRequest* requests[] = { &offline, &realtime, &upload };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::BaseRequest* request) {
        vis::Client client("localhost", PORT);
        BOOST_REQUIRE(client.probe());

        vis::Response response = client.sendRequest(request);
        BOOST_CHECK(response.results.empty()); // FIXME complete
    });
    server.stop();
}

BOOST_AUTO_TEST_SUITE_END();

