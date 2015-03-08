/**
 * @file descriptors_type.hpp
 * @brief Visual image search supported descriptors.
 * @author Paolo D'Apice
 */

#ifndef VIS_DESCRIPTORS_TYPE_HPP
#define VIS_DESCRIPTORS_TYPE_HPP

#include <string>

namespace vis {

// TODO use a class like Java Enum types.

/// Available descriptors types.
enum DescriptorsType {
    HOG,        ///< HOG descriptors.
    HSV,        ///< HSV color histograms.
    HOG_HSV,    ///< Both HOG descriptors and HSV color histograms.
};

/// Returns the length of a vector for the given type.
inline size_t
getLength(DescriptorsType type) {
    switch(type) {
        case HSV:
            return 166;
        case HOG:
            return 300;
        case HOG_HSV:
            return 466;
    }
}

/// Returns a textual representation of the enum value.
inline std::string
toString(DescriptorsType type) {
    switch(type) {
        case HSV:
            return "HSV";
        case HOG:
            return "HOG";
        case HOG_HSV:
            return "HOG_HSV";
    }
}

/// Returns enum value from string.
inline DescriptorsType
toDescriptorsType(const std::string& type) {
    if (type == "HSV" or type == "color")
        return HSV;
    else if (type == "HOG" or type == "shape")
        return HOG;
    else if (type == "HOG_HSV" or type == "combined")
        return HOG_HSV;
    else
        throw "invalid type";
}

/// Returns \c true if the given type requires a vocabulary.
inline bool
requiresVocabulary(DescriptorsType type) {
    return (type == HOG or type == HOG_HSV);
}

/// Returns a more friendly description of the enum type.
/// To be used in the UI.
inline std::string
typeString(DescriptorsType type) {
    switch (type) {
    case HOG:
        return "shape";
    case HSV:
        return "color";
    case HOG_HSV:
        return "combined";
    }
}

} /* namespace vis */

#endif /* VIS_DESCRIPTORS_TYPE_HPP */

