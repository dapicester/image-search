/**
 * @file perf_utils.hpp
 * @brief Utilities.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_UTILS_HPP
#define VIS_PERF_UTILS_HPP

#include "vis/descriptors_type.hpp"

#include <armadillo>
#include <opencv2/core/core.hpp>

namespace perf {

inline arma::fmat
getRandomData(size_t rows, size_t cols) {
    return arma::randu<arma::fmat>(rows, cols);
}

// TODO move this to descriptors_type.hpp ?
inline size_t
getLength(vis::DescriptorsType type) {
    switch(type) {
        case vis::HSV: return 166;
        case vis::HOG: return 300;
        case vis::HOG_HSV: return 466;
    }
}

} /* namespace perf */

#endif /* VIS_PERF_UTILS_HPP */

