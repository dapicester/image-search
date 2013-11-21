/**
 * @file test_protocol.cpp
 * @brief Unit test for the image search protocol.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE protocol
#include <boost/test/unit_test.hpp>

#include "protocol_serialization.hpp"

namespace vis {

inline bool
operator==(const vis::Request& left, const vis::Request& right) {
    return left.requestType == right.requestType
        and left.category == right.category
        and left.queryType == right.queryType
        and left.numResults == right.numResults
        and (left.id == right.id or left.descriptors == right.descriptors);
}

inline bool
operator==(const vis::Response left, const vis::Response& right) {
    return left.results == right.results;
}

} // namespace vis

static const vis::Request offline  = { 'o', "bag", 'c', 20, 42 };
//static const vis::Request realtime = { 'r', "bag", 'c', 20, std::vector<float>(166, 0.f) };

#define PRINT(X) std::cout << #X << ": " << X << std::endl;

BOOST_AUTO_TEST_CASE(serialize_request) {
    vis::Request requests[] = { offline/*, realtime*/ };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::Request& request) {
        // serialize
        boost::asio::streambuf buf;
        vis::put(buf, request);

        // deserialize
        vis::Request deserialized;
        vis::get(buf, deserialized);

        PRINT(request);
        PRINT(deserialized);

        BOOST_CHECK_EQUAL(request, deserialized);
    });
}

