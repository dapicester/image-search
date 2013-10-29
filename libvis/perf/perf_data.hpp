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
#include <boost/lexical_cast.hpp>

namespace perf {

typedef std::vector<timestamp_t> TimestampVector;
typedef std::map<std::string, std::string> ParamsMap;

/// @brief Execution times for a single index configuration.
struct Timings {
    ParamsMap params;        ///< Test parameters.
    TimestampVector timings; ///< Execution times.

    template <typename T>
    void addParam(const std::string& name, T value) {
        params[name] = boost::lexical_cast<std::string>(value);
    }

    void setTimings(const TimestampVector& values) {
        timings = values;
    }

private:
    friend class boost::serialization::access;

    /// XML serialization.
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(params);
        ar & BOOST_SERIALIZATION_NVP(timings);
    }
};

/// @brief Shortcut.
typedef std::vector<Timings> TimingsVector;

} /* namespace perf */

#endif /* VIS_PERF_DATA_HPP */

