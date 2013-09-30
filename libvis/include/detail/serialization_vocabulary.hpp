/**
 * @file detail/serialization_vocabulary.hpp
 * @brief Visual word vocabular serialization using Boost Serialization
 * @author Paolo D'Apice
 */

#ifndef VIS_DETAILS_SERIALIZATION_VOCABULARY_HPP
#define VIS_DETAILS_SERIALIZATION_VOCABULARY_HPP

#include <boost/serialization/scoped_ptr.hpp>

namespace vis {

template <typename Archive>
void
Vocabulary::serialize(Archive& ar, const unsigned int version) {
    ar & category;
    ar & numWords;
    ar & words;
    ar & kdtree;
}

} /* namespace vis */

#endif /* VIS_DETAILS_SERIALIZATION_VOCABULARY_HPP */

