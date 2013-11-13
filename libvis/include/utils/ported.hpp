/**
 * @file utils/ported.hpp
 * @brief Collections of functions ported from Matlab.
 * @author Paolo D'Apice
 */

#ifndef VIS_UTILS_PORTED_HPP
#define VIS_UTILS_PORTED_HPP

#include <armadillo>
#include <boost/assert.hpp>

namespace vis {

/**
 * @brief Select a given number of columns
 */
template <typename T>
arma::Mat<T>
subset(const arma::Mat<T>& in, size_t num) {
    size_t m = in.n_cols;
    size_t n = std::min(m, num);

    arma::uvec idx = arma::linspace<arma::uvec>(0, m-1, n);
    arma::Mat<T> out = in.cols(idx);
    BOOST_ASSERT(out.n_cols == n);

    return out;
}

template <typename T>
std::vector<T>
subset(const std::vector<T>& in, size_t num) {
    size_t m = in.size();
    size_t n = std::min(m, num);

    arma::uvec idx = arma::linspace<arma::uvec>(0, m-1, n);

    std::vector<T> out;
    for (arma::uvec::iterator it = idx.begin(); it != idx.end(); ++it) {
        out.push_back(in[*it]);
    }
    BOOST_ASSERT(out.size() == n);

    return out;
}

} /* namespace vis */

#endif /* VIS_UTILS_PORTED_HPP */

