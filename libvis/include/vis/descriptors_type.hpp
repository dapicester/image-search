/**
 * @file descriptors_type.hpp
 * @brief Visual image search supported descriptors.
 * @author Paolo D'Apice
 */

#ifndef VIS_DESCRIPTORS_TYPE_HPP
#define VIS_DESCRIPTORS_TYPE_HPP

namespace vis {

/// Available descriptors types.
enum DescriptorsType {
    HOG,        ///< HOG descriptors.
    HSV,        ///< HSV color histograms.
    HOG_HSV,    ///< Both HOG descriptors and HSV color histograms.
};

} /* namespace vis */

#endif /* VIS_DESCRIPTORS_TYPE_HPP */

