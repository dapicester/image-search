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
#include <opencv2/core/core.hpp>

namespace vis {

template <typename T>
class KMeans {
public:
    KMeans();
    ~KMeans();

    /**
     * @brief Cluster data
     * @param data Input data of size RxC containing C samples with dimension R
     * @param numCenters Number of centers
     * @return Cluster matrix of size numCenters x C
     */
    cv::Mat cluster(const cv::Mat& data, vl_size numCenters);

private:
    VlKMeans* kmeans;
};

} /* namespace vis */

#include "detail/kmeans_impl.hpp"

#endif /* VIS_KMEANS_HPP */

