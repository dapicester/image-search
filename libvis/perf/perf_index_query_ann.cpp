/**
 * @file perf_index_query_ann.cpp
 * @brief Simple benchmark on index querying.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

static const size_t NUM_DATA           = 1e5;
static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN                = perf::getLength(TYPE);
static const size_t NUM_TREES          = 4;
static const size_t NUM_NEIGHBORS      = 15;

static const std::string SAVE_FILE     = "query_index_ann.xml";

/// @brief Index query time vs. max number of comparisons.
int main(int, char**) {
    perf::TimingsVector results;

    cv::Mat data = perf::getRandomData(LEN, NUM_DATA);
    vis::Index index;
    index.build("benchmark", data, TYPE, NUM_TREES);

    BOOST_FOREACH(size_t maxComparisons, perf::MAX_COMPARISONS) {
        perf::Timings t;
        t.addParam("numData", NUM_DATA);
        t.addParam("length", LEN);
        t.addParam("numTrees", NUM_TREES);
        t.addParam("numNeigbors", NUM_NEIGHBORS);
        t.addParam("maxComparisons", maxComparisons);
        t.setTimings(perf::queryIndex(index, data, NUM_NEIGHBORS, maxComparisons));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

