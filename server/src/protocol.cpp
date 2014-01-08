/**
 * @file protocol.cpp
 * @brief Communication protocol used in the image search.
 * @author Paolo D'Apice
 */

#include "protocol.hpp"
#include <boost/serialization/export.hpp>
#include <iostream>

namespace vis {

BaseRequest::BaseRequest() {}

BaseRequest::BaseRequest(RequestType rt, const std::string& cat,
            char t, unsigned n)
        : requestType(rt), category(cat), queryType(t), numResults(n) {}

BaseRequest::~BaseRequest() {}

bool
BaseRequest::equals(const BaseRequest& o) const {
    return requestType == o.requestType
        and category == o.category
        and queryType == o.queryType
        and numResults == o.numResults;
}

void
BaseRequest::print(std::ostream& os) const {
    os << static_cast<char>(requestType) << ' '
        << category << ' '
        << queryType << ' ' << numResults;
}

OfflineRequest::OfflineRequest() {}

OfflineRequest::OfflineRequest(RequestType requestType, const std::string& category,
            char queryType, unsigned numResults, unsigned i)
        : BaseRequest(RequestType::OFFLINE, category, queryType, numResults), id(i) {}

OfflineRequest::~OfflineRequest() {}

void
OfflineRequest::print(std::ostream& os) const {
    BaseRequest::print(os);
    os << ' ' << id;
}

bool
OfflineRequest::equals(const BaseRequest& o) const {
    return BaseRequest::equals(o)
        and id == dynamic_cast<const OfflineRequest&>(o).id;
}

RealtimeRequest::RealtimeRequest() {}

RealtimeRequest::RealtimeRequest(RequestType requestType, const std::string& category,
            char queryType, unsigned numResults, const std::vector<float>& d)
        : BaseRequest(RequestType::REALTIME, category, queryType, numResults), descriptors(d) {}

RealtimeRequest::~RealtimeRequest() {}

void
RealtimeRequest::print(std::ostream& os) const {
    BaseRequest::print(os);
    os << ' ' << descriptors;
}

bool
RealtimeRequest::equals(const BaseRequest& o) const {
    return BaseRequest::equals(o)
        and descriptors == dynamic_cast<const RealtimeRequest&>(o).descriptors;
}

UploadRequest::UploadRequest() {}

UploadRequest::UploadRequest(RequestType requestType, const std::string& category,
            char queryType, unsigned numResults, void* data)
        : BaseRequest(RequestType::UPLOAD, category, queryType, numResults) {}

UploadRequest::~UploadRequest() {}

void
UploadRequest::print(std::ostream& os) const {
    BaseRequest::print(os);
    //TODO print upload request
}

bool
UploadRequest::equals(const BaseRequest& o) const {
    std::cout<<"up"<<std::endl;
    return BaseRequest::equals(o);
    // TODO equals upload request
}

Response::Response() {}

Response::Response(ResponseStatus st, const std::string& msg,
            const std::vector<id_type>& res)
        : status(st), message(msg), results(res) {}

Response::~Response() {}

} // namespace vis

