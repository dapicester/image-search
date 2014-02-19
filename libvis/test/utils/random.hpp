/**
 * @file utils/random.hpp
 * @brief Random number generator
 * @author Paolo D'Apice
 */

#ifndef VIS_TEST_UTILS_RANDOM_HPP
#define VIS_TEST_UTILS_RANDOM_HPP

#include "vis/traits.hpp"

extern "C" {
#include <vl/random.h>
}

namespace vis {

/// Random number generator
template <typename T>
class Random {
public:
    Random();
    T next();
private:
    VlRand* rand;
};

/* implementation */

template <typename T>
Random<T>::Random() {
    rand = vl_get_rand();
}

template <typename T>
T
Random<T>::next() {
    switch (vis::VlType<T>::type) {
    case VL_TYPE_FLOAT:
    case VL_TYPE_DOUBLE:
        return static_cast<T>(vl_rand_real1(rand));
    case VL_TYPE_INT32:
        return static_cast<T>(vl_rand_int31(rand));
    }
}

} /* namespace vis */

#endif /* VIS_TEST_UTILS_RANDOM_HPP */

