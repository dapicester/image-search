/**
 * @file test_protocol.cpp
 * @brief Unit test for the image search protocol.
 * @author Paolo D'Apice
 */

#define BOOST_TEST_MODULE protocol
#include <boost/test/unit_test.hpp>

#include "protocol_serialization.hpp"

#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(vis::OfflineRequest);
BOOST_CLASS_EXPORT(vis::RealtimeRequest);
BOOST_CLASS_EXPORT(vis::UploadRequest);

namespace vis {

inline bool
operator==(const vis::BaseRequest& left, const vis::BaseRequest& right) {
    return left.equals(right);
}

inline bool
operator==(const vis::Response left, const vis::Response& right) {
    return left.results == right.results;
}

} // namespace vis

static const vis::OfflineRequest offline(vis::RequestType::OFFLINE, "bag", 'c', 20, 42);
static const vis::RealtimeRequest realtime(vis::RequestType::REALTIME, "bag", 'c', 20, std::vector<float>(166, 0.f));
static const vis::UploadRequest upload( vis::RequestType::UPLOAD, "bag", 'c', 20, nullptr /*image data*/);

#define PRINT(X) std::cout << #X << ": " << X << std::endl;

BOOST_AUTO_TEST_CASE(serialize_request) {
    const vis::BaseRequest* requests[] = { &offline, &realtime, &upload };
    std::for_each(std::begin(requests), std::end(requests), [](const vis::BaseRequest* request) {
        boost::asio::streambuf buf;

        // serialize
        vis::put(buf, request);

        // deserialize
        vis::BaseRequest* deserialized;
        vis::get(buf, deserialized);

        PRINT(*request);
        PRINT(*deserialized);

        BOOST_CHECK_EQUAL(*request, *deserialized);
        delete deserialized;
    });
}

