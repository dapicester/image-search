/**
 * @file request_handler.hpp
 * @brief Image search server request handler.
 * @author Paolo D'Apice
 */

#ifndef VIS_REQUEST_HANDLER_HPP
#define VIS_REQUEST_HANDLER_HPP

#include <boost/noncopyable.hpp>
#include <string>

namespace vis {

class BaseRequest;
class Response;

namespace server {

/// The common handler for all incoming requests.
class RequestHandler : private boost::noncopyable {
public:

    /// Construct with ...
    explicit RequestHandler(/*const std::string& doc_root*/);

    /// Handle a request and produce a reply.
    // XXX can use references instead of pointer?
    void handle(const vis::BaseRequest* req, vis::Response& res);

private:
    // TODO ImageSearch instance
};

} // namespace server
} // namespace vis

#endif // VIS_REQUEST_HANDLER_HPP

