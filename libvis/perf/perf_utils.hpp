/**
 * @file perf_utils.hpp
 * @brief Utilities.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_UTILS_HPP
#define VIS_PERF_UTILS_HPP

#include "descriptors_type.hpp"
#include <opencv2/core/core.hpp>

namespace perf {

cv::Mat
getRandomData(size_t rows, size_t cols, float min = 0., float max = 1.) {
    cv::Mat data(rows, cols, cv::DataType<float>::type);
    cv::randu(data, min, max);
    return data;
}


// TODO move this to descriptors_type.hpp
size_t
getLength(vis::DescriptorsType type) {
    switch(type) {
        case vis::HSV: return 166;
        case vis::HOG: return 300;
        case vis::HOG_HSV: return 466;
    }
}

} /* namespace perf */

#endif /* VIS_PERF_UTILS_HPP */

