/**
 * @file protocol.hpp
 * @brief Communication protocol used in the image search.
 * @author Paolo D'Apice
 */

#ifndef VIS_PROTOCOL_HPP
#define VIS_PROTOCOL_HPP

#include <iterator>
#include <sstream>
#include <vector>

namespace vis {

enum class RequestType : char {
    OFFLINE  = 'o',
    REALTIME = 'r',
    UPLOAD   = 'u'
};

// TODO factory method for request objects

struct BaseRequest {
    BaseRequest();
    BaseRequest(RequestType requestType,
            const std::string& category,
            char queryType,
            unsigned numResults);
    virtual ~BaseRequest();

    virtual void print(std::ostream&) const;
    virtual bool equals(const BaseRequest&) const;

    RequestType requestType;
    std::string category;
    char queryType;
    unsigned numResults;
};

struct OfflineRequest : BaseRequest {
    OfflineRequest();
    OfflineRequest(RequestType requestType,
            const std::string& category,
            char queryType,
            unsigned numResults,
            unsigned id);
    ~OfflineRequest();

    void print(std::ostream&) const;
    bool equals(const BaseRequest&) const;

    unsigned id;
};

struct RealtimeRequest : BaseRequest {
    RealtimeRequest();
    RealtimeRequest(RequestType requestType,
            const std::string& category,
            char queryType,
            unsigned numResults,
            const std::vector<float>& descriptors);
    ~RealtimeRequest();

    void print(std::ostream&) const;
    bool equals(const BaseRequest&) const;

    std::vector<float> descriptors;
};

struct UploadRequest : BaseRequest {
    UploadRequest();
    UploadRequest(RequestType requestType,
            const std::string& category,
            char queryType,
            unsigned numResults,
            void* data);
    ~UploadRequest();

    void print(std::ostream&) const;
    bool equals(const BaseRequest&) const;

    // TODO opencv image
};

struct Response {
    std::vector<unsigned> results;
};

template <typename T>
inline std::ostream&
operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, ","));
    os << "\b]";
    return os;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::BaseRequest& r) {
    r.print(os);
    return os;
}

inline std::ostream&
operator<<(std::ostream& os, const vis::Response& r) {
    return os << r.results;
}

} // namespace vis

#endif // VIS_PROTOCOL_HPP

