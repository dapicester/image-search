/**
 * @file perf_index_query_neighbors.cpp
 * @brief Simple benchmark on index querying.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

static const size_t NUM_DATA           = 1e5;
static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN                = vis::getLength(TYPE);

static const std::string SAVE_FILE     = "query_index_neighbors.xml";

/// @brief Index query time vs. number of neighbors.
int main(int, char**) {
    perf::TimingsVector results;

    arma::fmat data = perf::getRandomData(LEN, NUM_DATA);
    vis::Index index;
    index.build("benchmark", data, TYPE);

    for (size_t numNeighbors : perf::NUM_NEIGHBORS) {
        perf::Timings t;
        t.addParam("numData", NUM_DATA);
        t.addParam("length", LEN);
        t.addParam("numNeigbors", numNeighbors);
        t.setTimings(perf::queryIndex(index, data, numNeighbors));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

