/**
 * @file test_protocol.cpp
 * @brief Unit test for the image search protocol.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE protocol
#include <boost/test/unit_test.hpp>

#include "protocol.hpp"

BOOST_AUTO_TEST_CASE(serialize_request) {
    const vis::Request requests[] = {
        { vis::RequestType::OFFLINE, "bag", vis::QueryType::COLOR, 20, 42, {} },
        { vis::RequestType::REALTIME, "bag", vis::QueryType::COLOR, 20, 0, std::vector<float>(166, 0.f) },
        { vis::RequestType::UPLOAD, "bag", vis::QueryType::COLOR, 20, 0, {} /* TODO image */},
    };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::Request& request) {
        boost::asio::streambuf buf;

        // serialize
        vis::put(buf, request);

        // deserialize
        vis::Request deserialized;
        vis::get(buf, deserialized);

        BOOST_CHECK_EQUAL(request, deserialized);
    });
}

BOOST_AUTO_TEST_CASE(serialize_response) {
    const vis::Response responses[] = {
        { vis::ResponseStatus::OK, { { 33, "path33" }, { 42, "path42" } } },
        { vis::ResponseStatus::ERROR }
    };
    std::for_each(std::begin(responses), std::end(responses), [](const vis::Response& response) {
        boost::asio::streambuf buf;

        // serialize
        vis::put(buf, response);

        // deserialize
        vis::Response deserialized;
        vis::get(buf, deserialized);

        BOOST_CHECK_EQUAL(response, deserialized);
    });
}

