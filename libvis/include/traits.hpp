/**
 * @file traits.hpp
 * @brief Traits for VLFeat
 * @author Paolo D'Apice
 */

#ifndef VIS_TRAITS_HPP
#define VIS_TRAITS_HPP

extern "C" {
#include <vl/generic.h>
}

namespace vis {

template <class T> struct VlType;

template <>
struct VlType<int> {
    static const vl_type type = VL_TYPE_INT32;
};

template <>
struct VlType<float> {
    static const vl_type type = VL_TYPE_FLOAT;
};

template <>
struct VlType<double> {
    static const vl_type type = VL_TYPE_DOUBLE;
};

}

#endif /* VIS_TRAITS_HPP */

