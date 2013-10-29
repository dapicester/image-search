/**
 * @file perf_results.cpp
 * @brief Produce performance tests results.
 * @author Paolo D'Apice
 */

#include "dir.h"
#include "perf_data.hpp"
#include "perf_serialization.hpp"
#include <boost/filesystem/path.hpp>

namespace fs = boost::filesystem;

/// Directory containing XML results.
static const fs::path XML_DIR = fs::path(DATA_DIR) / "perf";

typedef std::pair<std::string, std::string> FileVariablePair;

static const boost::array<FileVariablePair, 6> PAIRS = {
        std::make_pair("build_index.xml", "numData"),
        std::make_pair("build_index_numtrees.xml", "numTrees"),
        std::make_pair("query_index.xml", "numData"),
        std::make_pair("query_index_numtrees.xml", "numTrees"),
        std::make_pair("query_index_neighbors.xml", "numNeigbors"), // TODO fix typo
        std::make_pair("query_index_ann.xml", "maxComparisons"),
        //std::make_pair("descriptors.xml", "descriptor")
};

/// Process XML results and make a table-like object.
void process(const std::string& input, const std::string& name) {
    fs::path file = XML_DIR / input;

    perf::TimingsVector timings;
    perf::load(file.string(), timings);

    perf::ResultsTable table(name, timings);
    perf::save("results_" + input, table);
}

int main(int, char**) {
    BOOST_FOREACH(const FileVariablePair& pair, PAIRS) {
        process(pair.first, pair.second);
    }
}

