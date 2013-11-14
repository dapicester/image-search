/**
 * @file kmeans.hpp
 * @brief VLFeat K-means wrapper
 * @author Paolo D'Apice
 */

#ifndef VIS_KMEANS_HPP
#define VIS_KMEANS_HPP

extern "C" {
#include <vl/kmeans.h>
}
#include <armadillo>

namespace vis {

/// @brief K-means object.
template <typename T>
class KMeans {
public:
    /// Default constructor.
    KMeans();

    /// Default destructor.
    ~KMeans();

    /**
     * @brief Cluster data.
     * @param data Input data of size RxC containing C samples with dimension R
     * @param numCenters Number of centers
     * @return Matrix of cluster centers with size numCenters x C
     */
    arma::Mat<T> cluster(const arma::Mat<T>& data, unsigned numCenters);

    /// Set verbosity.
    void setVerbose(bool val) { verbose = val; }

private:
    bool verbose = false;
    VlKMeans* kmeans;
};

} /* namespace vis */

#include "detail/kmeans_impl.hpp"

#endif /* VIS_KMEANS_HPP */

