/**
 * @file perf_index_query.cpp
 * @brief Simple benchmark on index querying.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

using namespace perf;

static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN = getLength(TYPE);
static const std::string SAVE_FILE = "query_index.xml";

/// @brief Index query time vs. number of records.
int main(int, char**) {
    IndexTimingsVector results;

    BOOST_FOREACH(size_t size, SIZES) {
        IndexTimings t;
        t.addParam("size", size);

        cv::Mat data = getRandomData(LEN, size);

        vis::Index index;
        index.build("benchmark", data, TYPE);

        t.setTimings(perfQueryIndex(index, data));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}
