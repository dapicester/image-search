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

} /* namespace perf */

#endif /* VIS_PERF_UTILS_HPP */

