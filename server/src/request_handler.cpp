/**
 * @file request_handler.cpp
 * @brief Image search server request handler.
 * @author Paolo D'Apice
 */

#include "request_handler.hpp"
#include "protocol.hpp"

#include <vis/configuration.hpp>
#include <vis/imsearch.hpp>
#include <vis/utils/filesystem.hpp>

#include <glog/logging.h>

namespace vis {
namespace server {

RequestHandler::RequestHandler(const vis::config::Configuration& config) {
    LOG(INFO) << "Initializing image search";

    std::for_each(config.categories.begin(),
                  config.categories.end(),
                  [&] (const vis::config::Category& category) {
        std::string name = category.name;
        for (auto type_str : category.type) {
            DescriptorsType type = toDescriptorsType(type_str);

            LOG(INFO) << "Adding " << name << "/" << type_str;
            ImageSearch* imsearch = new ImageSearch(name, type, category.dir);
            imsearch->load();

            service.insert(name, imsearch);
        }
    });

}

RequestHandler::~RequestHandler() {}

void
RequestHandler::handle(const vis::Request& req, vis::Response& res) {
    DLOG(INFO) << "Handling request: " << req;

    if (!service.count(req.category)) {
        res.status = vis::ResponseStatus::NO_CATEGORY;

        LOG(INFO) << "Response: " << res;
        return;
    }

    auto range = service.equal_range(req.category);
    auto it = range.begin();
    for (auto end = range.end(); it != end; ++it) {
        vis::DescriptorsType type = it->second->getType();
        QueryType t = req.queryType;
        // XXX quick'n dirty (TM)
        if ((t == QueryType::COLOR and type == vis::HSV)
                or (t == QueryType::SHAPE and type == vis::HOG)
                or (t == QueryType::COLOR_SHAPE and type == vis::HOG_HSV))
            break;
    }

    if (it == range.end()) {
        res.status = vis::ResponseStatus::NO_QUERY_TYPE;

        LOG(INFO) << "Response: " << res;
        return;
    }

    const vis::ImageSearch& imsearch = *(it->second);
    std::vector<id_type> results;

    switch (req.type) {
    case vis::RequestType::OFFLINE:
        LOG(INFO) << "Offline: id=" << req.id;
        imsearch.query(req.id, results, req.numResults);
        break;
    case vis::RequestType::REALTIME:
        LOG(INFO) << "Realtime: descriptors=" << req.descriptors.size();
        imsearch.query(req.descriptors, results, req.numResults);
        break;
    case vis::RequestType::UPLOAD:
        LOG(INFO) << "Upload: image=TODO";
        // TODO imsearch.query(req.image, results, req.numResults);
        break;
    }

    res.status = vis::ResponseStatus::OK;

    auto paths = imsearch.get(results);
    for (auto i = 0; i < paths.size(); ++i) {
        Match m = { results[i], paths[i].string() };
        res.results.push_back(m);
    };

    DLOG(INFO) << "Response: " << res;
}

} // namespace server
} // namespace vis

