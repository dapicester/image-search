/**
 * @file perf_data.hpp
 * @brief Performance test data structures.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_DATA_HPP
#define VIS_PERF_DATA_HPP

#include "descriptors_type.hpp"
#include "perf_serialization.hpp"
#include "utils/posixtimer.hpp"

namespace perf {

/// @brief Execution times for a single index configuration.
struct IndexTimings {
    size_t size;                      ///< Number of records.
    vis::DescriptorsType type;        ///< Actual descriptor.
    std::vector<timestamp_t> timings; ///< Execution times.

private:
    friend class boost::serialization::access;

    /// XML serialization.
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::make_nvp("size", size);
        ar & boost::serialization::make_nvp("type", type);
        ar & boost::serialization::make_nvp("timings", timings);
    }
};

/// @brief Shortcut.
typedef std::vector<IndexTimings> IndexTimingsVector;

} /* namespace perf */

#endif /* VIS_PERF_DATA_HPP */

