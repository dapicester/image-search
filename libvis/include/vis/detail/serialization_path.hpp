/**
 * @file detail/serialization_path.hpp
 * @brief Boost Filesystem Path serialization using Boost Serialization.
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAILS_SERIALIZATION_PATH_HPP
#define VIS_DETAILS_SERIALIZATION_PATH_HPP

#include <armadillo>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/split_free.hpp>

BOOST_SERIALIZATION_SPLIT_FREE(boost::filesystem::path)

namespace boost { namespace serialization {

/// @brief Boost serialization for @a boost::filesystem::path.
template <typename OutputArchive>
void
save(OutputArchive& ar, const boost::filesystem::path& path, const unsigned int version) {
    ar & path.string();
}

/// @brief Boost deserialization for @a boost::filesystem::path.
template <typename InputArchive>
void
load(InputArchive& ar, boost::filesystem::path& path, const unsigned int version) {
    std::string string;
    ar & string;

    path = string;
}

}} /* namespace boost::serialization */

#endif /* VIS_DETAILS_SERIALIZATION_PATH_HPP */

