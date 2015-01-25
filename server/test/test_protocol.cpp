/**
 * @file test_protocol.cpp
 * @brief Unit test for the image search protocol.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE protocol
#include <boost/test/unit_test.hpp>

#include "protocol.hpp"

vis::Request offline() {
    vis::Request req;
    req.type = vis::RequestType::OFFLINE;
    req.category = "bag";
    req.queryType = 'c';
    req.numResults = 20;
    req.id = 42;
    return req;
}

vis::Request realtime() {
    vis::Request req;
    req.type = vis::RequestType::REALTIME;
    req.category = "bag";
    req.queryType = 'c';
    req.numResults = 20;
    req.descriptors = std::vector<float>(166, 0.f);
    return req;
}

vis::Request upload() {
    vis::Request req;
    req.type = vis::RequestType::UPLOAD;
    req.category = "bag";
    req.queryType = 'c';
    req.numResults = 20;
    // TODO req.descriptors = image;
    return req;
}

#define PRINT(X) std::cout << #X << ": " << X << std::endl;

BOOST_AUTO_TEST_CASE(serialize_request) {
    const vis::Request requests[] = { offline()/*, realtime(), &upload()*/ };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::Request& request) {
        boost::asio::streambuf buf;

        // serialize
        vis::put(buf, request);

        // deserialize
        vis::Request deserialized;
        vis::get(buf, deserialized);

        PRINT(request);
        PRINT(deserialized);

        BOOST_CHECK_EQUAL(request, deserialized);
    });
}

#if 0
static const vis::Response ok(vis::ResponseStatus::OK, "", { 0 }, { "" });
static const vis::Response error(vis::ResponseStatus::OK, "message", {}, {});

BOOST_AUTO_TEST_CASE(serialize_response) {
    const vis::Response responses[] = { ok, error };
    std::for_each(std::begin(responses), std::end(responses), [](const vis::Response response) {
        boost::asio::streambuf buf;

        // serialize
        vis::put(buf, response);

        // deserialize
        vis::Response deserialized;
        vis::get(buf, deserialized);

        PRINT(response);
        PRINT(deserialized);

        BOOST_CHECK_EQUAL(response, deserialized);
    });
}
#endif

