/**
 * @file protocol.hpp
 * @brief Communication protocol used in the image search.
 * @author Paolo D'Apice
 */

#ifndef VIS_PROTOCOL_HPP
#define VIS_PROTOCOL_HPP

#include <iterator>
#include <iostream>
#include <vector>

namespace vis {

typedef size_t id_type; // XXX vis::Index::id_type

enum RequestType { // TODO move into Request
    OFFLINE,
    REALTIME,
    UPLOAD,
};

struct Request {
    RequestType type;
    std::string category;
    char queryType; // XXX enum, string or DescriptorsType
    unsigned numResults;

    id_type id; // offline
    std::vector<float> descriptors; // realtime
    // TODO opencv image

    bool equals(const Request&) const;
};

enum ResponseStatus { // TODO move into Response
    OK,
    ERROR,
};

struct Response {
    ResponseStatus status;
    std::string message;
    std::vector<id_type> results;
    std::vector<std::string> paths;
};

inline bool
operator==(const Request& left, const Request& right) {
    return left.type == right.type
        and left.category == right.category
        and left.queryType == right.queryType
        and left.numResults == right.numResults
        and (left.id == right.id or left.descriptors == right.descriptors);
}

inline bool
operator==(const vis::Response& left, const vis::Response& right) {
    return left.status == right.status
        and left.message == right.message
        and left.results == right.results
        and left.paths == right.paths;
}

template <typename T>
inline std::ostream&
operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, ","));
    if (not v.empty()) os << "\b";
    os << "]";
    return os;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Request& r) {
    os << r.type << r.category << r.queryType << r.numResults;
    switch (r.type) {
        case vis::RequestType::OFFLINE:
            os << r.id;
            break;
        case vis::RequestType::REALTIME:
            os << r.descriptors;
            break;
        case vis::RequestType::UPLOAD:
            // TODO
            break;
    }
    return os;
}

inline std::istream&
operator>>(std::istream& is, vis::Request& r) {
    int t;
    is >> t >> r.category >> r.queryType >> r.numResults;
    switch (t) {
        case 0:
            r.type = vis::RequestType::OFFLINE;
            is >> r.id;
            break;
        case 1:
            r.type = vis::RequestType::REALTIME;
            // TODO is >> r.descriptors;
            break;
        case 2:
            r.type = vis::RequestType::UPLOAD;
            // TODO
            break;
    }
    return is;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Response& r) {
    return os << r.status << r.message << r.results << r.paths;
}

} // namespace vis

#endif // VIS_PROTOCOL_HPP

