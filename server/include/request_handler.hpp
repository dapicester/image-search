/**
 * @file request_handler.hpp
 * @brief Image search server request handler.
 * @author Paolo D'Apice
 */

#ifndef VIS_REQUEST_HANDLER_HPP
#define VIS_REQUEST_HANDLER_HPP

#include "configuration.hpp"

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_map.hpp>

namespace vis {

class ImageSearch;

struct BaseRequest;
struct OfflineRequest;
struct RealtimeRequest;
struct UploadRequest;
struct Response;

namespace server {

/// The common handler for all incoming requests.
class RequestHandler : private boost::noncopyable {
public:

    /// Construct with path to image search data dir.
    RequestHandler(const Configuration& config);

    /// Destructor
    ~RequestHandler();

    /// Handle a request and produce a reply.
    void handle(const vis::BaseRequest& req, vis::Response& res);

private:
    void doHandle(const vis::OfflineRequest& req, vis::Response& res);
    void doHandle(const vis::RealtimeRequest& req, vis::Response& res);
    void doHandle(const vis::UploadRequest& req, vis::Response& res);

private:
    boost::ptr_map<std::string, vis::ImageSearch> service;
};

} // namespace server
} // namespace vis

#endif // VIS_REQUEST_HANDLER_HPP

