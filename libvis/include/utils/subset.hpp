/**
 * @file utils/ported.hpp
 * @brief Collections of functions ported from Matlab.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_SUBSET_HPP
#define VIS_UTILS_SUBSET_HPP

#include <armadillo>
#include <boost/assert.hpp>

namespace vis {

/// @brief Select a given number of columns from matrix.
template <typename T>
arma::Mat<T>
subset(const arma::Mat<T>& in, size_t num) {
    const size_t m = in.n_cols;
    const size_t n = std::min(m, num);

    const arma::uvec idx = arma::linspace<arma::uvec>(0, m-1, n);
    arma::Mat<T> out = in.cols(idx);
    BOOST_ASSERT(out.n_cols == n);

    return out;
}

/// @brief Select a given number of elements from vector.
template <typename T>
std::vector<T>
subset(const std::vector<T>& in, size_t num) {
    const size_t m = in.size();
    const size_t n = std::min(m, num);

    const arma::uvec idx = arma::linspace<arma::uvec>(0, m-1, n);
    std::vector<T> out;
    std::for_each(idx.begin(), idx.end(), [&](unsigned i) { out.push_back(in[i]); });
    BOOST_ASSERT(out.size() == n);

    return out;
}

} /* namespace vis */

#endif /* VIS_UTILS_SUBSET_HPP */

