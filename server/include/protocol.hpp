/**
 * @file protocol.hpp
 * @brief Communication protocol used in the image search.
 * @author Paolo D'Apice
 */

#ifndef VIS_PROTOCOL_HPP
#define VIS_PROTOCOL_HPP

#include <boost/asio/streambuf.hpp>
#include <msgpack.hpp>
#include <iostream>
#include <iterator>
#include <vector>

namespace vis {

enum RequestType { // TODO move into Request
    OFFLINE,
    REALTIME,
    UPLOAD
};

enum QueryType { // TODO move into Request
    COLOR,
    SHAPE,
    COLOR_SHAPE
};

} // namespace vis

MSGPACK_ADD_ENUM(vis::RequestType);
MSGPACK_ADD_ENUM(vis::QueryType);

namespace vis {

typedef size_t id_type; // XXX vis::Index::id_type

struct Request {
    RequestType type;
    std::string category;
    QueryType queryType;
    unsigned numResults;

// TODO boost::variant?
    id_type id; // offline
    std::vector<float> descriptors; // realtime
    // TODO opencv image

    MSGPACK_DEFINE(type, category, queryType, numResults, id, descriptors);
};


enum ResponseStatus { // TODO move into Response
    OK,
    ERROR,
    NO_CATEGORY,
    NO_QUERY_TYPE
};

} // namespace vis

MSGPACK_ADD_ENUM(vis::ResponseStatus);

namespace vis {

struct Match { // TODO move into Response
    id_type id;
    std::string path;

    MSGPACK_DEFINE(id, path);
};

struct Response {
    ResponseStatus status;
    std::vector<Match> results;

    MSGPACK_DEFINE(status, results);
};

inline bool
operator==(const Request& left, const Request& right) {
    bool b = left.type == right.type
        and left.category == right.category
        and left.queryType == right.queryType
        and left.numResults == right.numResults;
    if (!b) return false;

    switch(left.type) {
        case vis::RequestType::OFFLINE:
            return left.id == right.id;
        case vis::RequestType::REALTIME:
            return left.descriptors == right.descriptors;
        case vis::RequestType::UPLOAD:
            // TODO
            return true;
    }
}

inline bool
operator==(const vis::Match& left, const vis::Match& right) {
    return left.id == right.id and left.path == right.path;
}

inline bool
operator==(const vis::Response& left, const vis::Response& right) {
    return left.status == right.status and left.results == right.results;
}

template <typename T>
inline std::ostream&
operator<<(std::ostream& os, const std::vector<T>& v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, " "));
    return os;
}

template <typename T>
inline std::istream&
operator>>(std::istream& is, std::vector<T>& v) {
    std::copy(std::istream_iterator<T>(is), std::istream_iterator<T>(), std::back_inserter(v));
    return is;
}

template <typename E>
typename std::enable_if<std::is_enum<E>::value, std::istream&>::type
operator>>(std::istream& is, E& e) {
    unsigned int value = 0;
    if (is >> value)
        e = static_cast<E>(value);
    return is;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Match& m) {
    return os << m.id << " " << m.path;
}

inline std::istream&
operator>>(std::istream& is, vis::Match& m) {
    return is >> m.id >> m.path;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Request& r) {
    os << r.type << ' ' << r.category << ' '
        << r.queryType << ' ' << r.numResults << ' ';
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
    is >> r.type >> r.category >> r.queryType >> r.numResults;
    switch (r.type) {
        case vis::RequestType::OFFLINE:
            is >> r.id;
            break;
        case vis::RequestType::REALTIME:
            is >> r.descriptors;
            break;
        case vis::RequestType::UPLOAD:
            // TODO
            break;
    }
    return is;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Response& r) {
    os << r.status << " " << r.results;
    return os;
}

inline std::istream&
operator>>(std::istream& is, vis::Response& r) {
    is >> r.status >> r.results;
    return is;
}

template <typename Object>
void
put(boost::asio::streambuf& buf, const Object& obj, bool msgpack = true) {
    if (msgpack) {
        msgpack::sbuffer sbuf;
        msgpack::pack(sbuf, obj);

        std::ostream os(&buf);
        os << sbuf.data();

        std::cout << sbuf.data() << std::endl;
    } else {
        std::ostream os(&buf);
        os << obj;
    }
}

template <typename Object>
void
get(boost::asio::streambuf& buf, Object& obj, bool msgpack = true) {
    std::istream is(&buf);
    if (msgpack) {
        std::string data;
        is >> data;

        std::cout << data << std::endl;

        msgpack::unpacked msg;
        msgpack::unpack(&msg, data.c_str(), data.size());

        msgpack::object o = msg.get();
        o.convert(&obj);
    } else {
        is >> obj;
    }
}

} // namespace vis

#endif // VIS_PROTOCOL_HPP

