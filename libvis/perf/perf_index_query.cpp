/**
 * @file perf_index_query.cpp
 * @brief Simple benchmark on index querying performance.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"
#include <cstdio>

using namespace perf;

int main(int, char**) {
    IndexTimingsVector results;
    results.resize(SIZES.size());

    for (int i = 0; i < SIZES.size(); i++) {
        IndexTimings t;
        t.size = SIZES[i];
        t.type = vis::HOG_HSV;

        size_t length = getLength(t.type);

        cv::Mat data = getRandomData(length, t.size);

        vis::Index index;
        index.build("benchmark", data, t.type);

        t.timings = perfQueryIndex(index, data);

        results[i] = t;
    }

    perf::save("query_index.xml", results);
}
