#ifndef VIS_PROTOCOL_HPP
#define VIS_PROTOCOL_HPP

#include <iterator>
#include <sstream>
#include <vector>

namespace vis {

/*
enum class RequestType : char {
    OFFLINE  = 'o',
    REALTIME = 'r',
    UPLOAD   = 'u'
};

// TODO abstract base class and three subclasses
// TODO factory method for request objects
*/

struct Request {
    char requestType;
    std::string category;
    char queryType;
    unsigned numResults;
    unsigned id;
    std::vector<float> descriptors;
};

struct Response {
    std::vector<unsigned> results;
};

template <typename T>
inline std::ostream&
operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, ","));
    os << "]";
    return os;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Request& r) {
    return os << static_cast<char>(r.requestType) << ' '
        << r.category << ' '
        << r.queryType << ' ' << r.numResults
        << r.id << '/' << r.descriptors;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Response& r) {
    return os << r.results;
}

} // namespace vis

#endif // VIS_PROTOCOL_HPP

