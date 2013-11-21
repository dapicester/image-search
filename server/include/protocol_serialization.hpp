#ifndef VIS_PROTOCOL_SERIALIZATION_HPP
#define VIS_PROTOCOL_SERIALIZATION_HPP

#include "protocol.hpp"

#include <boost/asio/streambuf.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace boost { namespace serialization {

template <typename Archive>
void
serialize(Archive & ar, vis::Request& req, const unsigned int version) {
    ar & req.requestType;
    ar & req.category;
    ar & req.queryType;
    ar & req.numResults;
    ar & req.id;
    ar & req.descriptors;
}

template <typename Archive>
void
serialize(Archive & ar, vis::Response& res, const unsigned int version) {
    ar & res.results;
}

}} // namespace boost::serialization

namespace vis {

template <typename Object>
void
put(boost::asio::streambuf& buf, const Object& obj) {
    std::ostream os(&buf);
    boost::archive::binary_oarchive ar(os);
    ar & obj;
}

template <typename Object>
void
get(boost::asio::streambuf& buf, Object& obj) {
    std::istream is(&buf);
    boost::archive::binary_iarchive ar(is);
    ar & obj;
}

} // namespace vis

#endif // VIS_PROTOCOL_SERIALIZATION_HPP

