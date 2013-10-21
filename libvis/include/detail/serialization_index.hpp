/**
 * @file serialization_index.hpp
 * @brief Index serialization/
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAIL_SERIALIZATION_INDEX_HPP
#define VIS_DETAIL_SERIALIZATION_INDEX_HPP

#include "index.hpp"

namespace vis {

template <typename Archive>
void
Index::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & type;
    ar & kdtree;
}

} /* namespace vis */

#endif /* VIS_DETAIL_SERIALIZATION_INDEX_HPP */

