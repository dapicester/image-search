/**
 * @file perf_data.hpp
 * @brief Performance test data structures.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_DATA_HPP
#define VIS_PERF_DATA_HPP

#include "descriptors_type.hpp"
#include "perf_serialization.hpp"
#include "utils/timer.hpp"
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <numeric>

namespace perf {

typedef std::vector<Timer::timestamp_t> TimestampVector;
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

/// @brief Compute mean and standard deviation of a vector of values.
template <typename T>
std::pair<double, double>
meanStDev(const std::vector<T>& v) {
    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();
#if 1
    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
#else
    std::vector<double> diff(v.size());
    std::transform(v.begin(), v.end(), diff.begin(),
               std::bind2nd(std::minus<double>(), mean));
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size());
#endif
    return std::make_pair(mean,stdev);
}

/// @brief Performance test results record.
struct Record {
    Record(double v, double m, double s) : value(v), mean(m), stdev(s) {}
private:
    double value;
    double mean;
    double stdev;

    friend class boost::serialization::access;

    /// XML serialization.
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(value);
        ar & BOOST_SERIALIZATION_NVP(mean);
        ar & BOOST_SERIALIZATION_NVP(stdev);
    }

    friend std::ostream& operator<<(std::ostream&, const Record&);
};

template <typename T>
std::ostream&
operator<<(std::ostream& os, const std::vector<T>& v) {
    std::copy(v.begin(), v.end(), std::ostream_iterator<T>(os, "\n"));
    return os;
}

std::ostream&
operator<<(std::ostream& os, const Record& r) {
    return os << r.value << ' ' << r.mean << ' ' << r.stdev;
}

/// @brief Performance test results.
struct ResultsTable {
    std::string title;
    std::vector<Record> records;

    ResultsTable() {}
    ResultsTable(const std::string& name, const TimingsVector& timings) {
        title = name;
        BOOST_FOREACH(const Timings& t, timings) {
            double value = boost::lexical_cast<double>(t.params.at(name));
            std::pair<double, double> ms = meanStDev(t.timings);
            records.push_back(Record(value, ms.first, ms.second));
        }
    }

private:
    friend class boost::serialization::access;

    /// XML serialization.
    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & BOOST_SERIALIZATION_NVP(title);
        ar & BOOST_SERIALIZATION_NVP(records);
    }
};

} /* namespace perf */

#endif /* VIS_PERF_DATA_HPP */

