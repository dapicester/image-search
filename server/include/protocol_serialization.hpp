/**
 * @file protocol_serialization.cpp
 * @brief Serialization implementation for the communication protocol used in the image search.
 * @author Paolo D'Apice
 */

#ifndef VIS_PROTOCOL_SERIALIZATION_HPP
#define VIS_PROTOCOL_SERIALIZATION_HPP

#include "protocol.hpp"

#include <boost/asio/streambuf.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/vector.hpp>

namespace boost { namespace serialization {

template <typename Archive>
void
serialize(Archive & ar, vis::BaseRequest& req, const unsigned int version) {
    ar & req.requestType;
    ar & req.category;
    ar & req.queryType;
    ar & req.numResults;
}

template <typename Archive>
void
serialize(Archive & ar, vis::OfflineRequest& req, const unsigned int version) {
    ar & base_object<vis::BaseRequest>(req);
    ar & req.id;
}

template <typename Archive>
void
serialize(Archive & ar, vis::RealtimeRequest& req, const unsigned int version) {
    ar & base_object<vis::BaseRequest>(req);
    ar & req.descriptors;
}

template <typename Archive>
void
serialize(Archive & ar, vis::UploadRequest& req, const unsigned int version) {
    ar & base_object<vis::BaseRequest>(req);
    // TODO
}

template <typename Archive>
void
serialize(Archive & ar, vis::Response& res, const unsigned int version) {
    ar & res.results;
}

}} // namespace boost::serialization

// FIXME put this in the main file (there must be only one otherwise duplicate symbols when linking)
//BOOST_CLASS_EXPORT(vis::OfflineRequest);
//BOOST_CLASS_EXPORT(vis::RealtimeRequest);
//BOOST_CLASS_EXPORT(vis::UploadRequest);

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

