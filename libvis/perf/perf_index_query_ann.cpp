/**
 * @file perf_index_query_ann.cpp
 * @brief Simple benchmark on index querying.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

using namespace perf;

static const size_t SIZE = 1e5;
static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN = getLength(TYPE);
static const size_t NEIGHBORS = 15;
static const std::string SAVE_FILE = "query_index_ann.xml";

/// @brief Index query time vs. max number of comparisons.
int main(int, char**) {
    IndexTimingsVector results;

    cv::Mat data = getRandomData(LEN, SIZE);

    vis::Index index;
    index.build("benchmark", data, TYPE);

    BOOST_FOREACH(size_t maxComparisons, MAX_COMPARISONS) {
        IndexTimings t;
        t.addParam("maxComparisons", maxComparisons);
        t.setTimings(perfQueryIndex(index, data, NEIGHBORS, maxComparisons));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}
