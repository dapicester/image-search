/**
 * @file perf_index_build_numtrees.cpp
 * @brief Simple benchmark on index building.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

static const size_t NUM_DATA           = 1e5;
static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN                = vis::getLength(TYPE);

static const std::string SAVE_FILE     = "build_index_numtrees.xml";

/// @brief Index build time vs. number of trees.
int main(int, char**) {
    perf::TimingsVector results;

    arma::fmat data = perf::getRandomData(LEN, NUM_DATA);

    for (size_t numTrees : perf::NUM_TREES) {
        perf::Timings t;
        t.addParam("numData", NUM_DATA);
        t.addParam("length", LEN);
        t.addParam("numTrees", numTrees);

        t.setTimings(perf::buildIndex(data, TYPE, numTrees));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

