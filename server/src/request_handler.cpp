/**
 * @file request_handler.cpp
 * @brief Image search server request handler.
 * @author Paolo D'Apice
 */

#include "request_handler.hpp"

#include "logging.hpp"
#include "protocol.hpp"

#include <vis/imsearch.hpp>
#include <vis/utils/filesystem.hpp>

#define _LOG(X) CLOG(X, "handler")

namespace vis {
namespace server {

RequestHandler::RequestHandler(const Configuration& config) {
    _LOG(INFO) << "Initializing image search";

    std::for_each(config.categories.begin(),
                  config.categories.end(),
                  [&] (const Category& category) {
        std::string name = category.name;
        for (auto type_str : category.type) {
            DescriptorsType type = toDescriptorsType(type_str);

            _LOG(INFO) << "Adding " << name << "/" << type_str;
            ImageSearch* imsearch = new ImageSearch(name, type, category.dir);
            imsearch->load();

            service.insert(name, imsearch);
        }
    });

}

RequestHandler::~RequestHandler() {}

void
RequestHandler::handle(const vis::BaseRequest& req, vis::Response& res) {
    _LOG(DEBUG) << "Handling request: " << req;

    if (!service.count(req.category)) {
        res.status = vis::ResponseStatus::ERROR;
        res.message = "Category not found";

        _LOG(DEBUG) << "Response: " << res;
        return;
    }

    auto range = service.equal_range(req.category);
    auto it = range.begin();
    for (auto end = range.end(); it != end; ++it) {
        vis::DescriptorsType type = it->second->getType();
        char t = req.queryType;
        // XXX quick'n dirty (TM)
        if ((t == 'c' and type == vis::HSV)
                or (t == 's' and type == vis::HOG)
                or (t == 'b' and type == vis::HOG_HSV))
            break;
    }

    if (it == range.end()) {
        res.status = vis::ResponseStatus::ERROR;
        res.message = "Query type not found";

        _LOG(DEBUG) << "Response: " << res;
        return;
    }

    const vis::ImageSearch& imsearch = *(it->second);

    switch (req.requestType) {
    case vis::RequestType::OFFLINE:
        doHandle(dynamic_cast<const vis::OfflineRequest&>(req), res, imsearch);
        break;
    case vis::RequestType::REALTIME:
        doHandle(dynamic_cast<const vis::RealtimeRequest&>(req), res, imsearch);
        break;
    case vis::RequestType::UPLOAD:
        doHandle(dynamic_cast<const vis::UploadRequest&>(req), res, imsearch);
        break;
    }

    res.status = vis::ResponseStatus::OK;
    toStrings(imsearch.get(res.results), res.paths);

    _LOG(DEBUG) << "Response: " << res;
}

void RequestHandler::doHandle(const vis::OfflineRequest& req,
        vis::Response& res, const ImageSearch& imsearch) {
    _LOG(DEBUG) << "Offline: id=" << req.id;
    imsearch.query(req.id, res.results, req.numResults);
}

void RequestHandler::doHandle(const vis::RealtimeRequest& req,
        vis::Response& res, const ImageSearch& imsearch) {
    _LOG(DEBUG) << "Realtime: descriptors=" << req.descriptors.size();
    imsearch.query(req.descriptors, res.results, req.numResults);
}

void RequestHandler::doHandle(const vis::UploadRequest& req,
        vis::Response& res, const ImageSearch& imsearch) {
    _LOG(DEBUG) << "Upload: image=TODO";
    // TODO imsearch.query(req.image, res.results, req.numResults);
}

} // namespace server
} // namespace vis

