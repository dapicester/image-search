/**
 * @file serialization.hpp
 * @brief Serialize performance test results.
 * @author Paolo D'Apice
 */

#ifndef VIS_PERF_SERIALIZATION_HPP
#define VIS_PERF_SERIALIZATION_HPP

#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>

namespace perf {

/// Save results to XML.
template <typename Object>
void save(const std::string& filename, Object results) {
    std::ofstream file(filename.c_str());
    {
        boost::archive::xml_oarchive ar(file, boost::archive::no_header);
        ar << BOOST_SERIALIZATION_NVP(results);
    }
    file.close();

    std::cout << "Results saved to file: " << filename << std::endl;
}

} /* namespace perf */

#endif /* VIS_PERF_SERIALIZATION_HPP */

