/**
 * @file descriptors_type.hpp
 * @brief Visual image search supported descriptors.
 * @author Paolo D'Apice
 */

#ifndef VIS_DESCRIPTORS_TYPE_HPP
#define VIS_DESCRIPTORS_TYPE_HPP

#include <string>

namespace vis {

/// Available descriptors types.
enum DescriptorsType {
    HOG,        ///< HOG descriptors.
    HSV,        ///< HSV color histograms.
    HOG_HSV,    ///< Both HOG descriptors and HSV color histograms.
};

/// Return a textual representation of the enum value.
inline std::string
toString(DescriptorsType type) {
    switch(type) {
        case vis::HSV:
            return "HSV";
        case vis::HOG:
            return "HOG";
        case vis::HOG_HSV:
            return "HOG_HSV";
    }
}

/// Return enum value from string.
inline DescriptorsType
toDescriptorsType(const std::string& type) {
    if (type == "HSV")
        return vis::HSV;
    else if (type == "HOG")
        return vis::HOG;
    else if (type == "HOG_HSV")
        return vis::HOG_HSV;
}

} /* namespace vis */

#endif /* VIS_DESCRIPTORS_TYPE_HPP */

