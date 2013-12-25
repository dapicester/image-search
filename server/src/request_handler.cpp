/**
 * @file request_handler.cpp
 * @brief Image search server request handler.
 * @author Paolo D'Apice
 */

#include "request_handler.hpp"

#include "logging.hpp"
#include "protocol.hpp"

#define _LOG(X) CLOG(X, "handler")

namespace vis {
namespace server {

RequestHandler::RequestHandler() {
    // TODO
}

void
RequestHandler::handle(const vis::BaseRequest* req, vis::Response& res) {
    _LOG(INFO) << "request: " << *req;

    // TODO process request here

    _LOG(INFO) << "response: " << res;
    delete req; // XXX
}

} // namespace server
} // namespace vis

