/**
 * @file request_handler.cpp
 * @brief Image search server request handler.
 * @author Paolo D'Apice
 */

#include "request_handler.hpp"

#include "logging.hpp"
#include "protocol.hpp"

#include <vis/imsearch.hpp>

#define _LOG(X) CLOG(X, "handler")

namespace vis {
namespace server {

RequestHandler::RequestHandler(const std::string& datadir) {
    _LOG(INFO) << "Initializing image search to dir: " << datadir;
    // TODO category/type
    service.reset(new vis::ImageSearch("bag", vis::DescriptorsType::HSV, datadir));
    service->load();
}

RequestHandler::~RequestHandler() {}

void
RequestHandler::handle(const vis::BaseRequest& req, vis::Response& res) {
    _LOG(DEBUG) << "Handling request" << req;

    // TODO process request here
    switch (req.requestType) {
    case vis::RequestType::OFFLINE:
        doHandle(dynamic_cast<const vis::OfflineRequest&>(req), res);
        break;
    case vis::RequestType::REALTIME:
        doHandle(dynamic_cast<const vis::RealtimeRequest&>(req), res);
        break;
    case vis::RequestType::UPLOAD:
        doHandle(dynamic_cast<const vis::UploadRequest&>(req), res);
        break;
    }

    _LOG(DEBUG) << "Response: " << res;
}

void RequestHandler::doHandle(const vis::OfflineRequest& req, vis::Response& res) {
    _LOG(DEBUG) << "Offline: id=" << req.id;
    service->query(req.id, res.results, req.numResults);
}

void RequestHandler::doHandle(const vis::RealtimeRequest& req, vis::Response& res) {
    _LOG(DEBUG) << "Realtime: descriptors=" << req.descriptors.size();
    service->query(req.descriptors, res.results, req.numResults);
}

void RequestHandler::doHandle(const vis::UploadRequest& req, vis::Response& res) {
    _LOG(DEBUG) << "Upload: image=TODO";
    // TODO service.query(req.image, res.results, req.numResults);
    res.results.resize(req.numResults);
}

} // namespace server
} // namespace vis

