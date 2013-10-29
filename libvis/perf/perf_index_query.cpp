/**
 * @file perf_index_query.cpp
 * @brief Simple benchmark on index querying.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN                = perf::getLength(TYPE);

static const std::string SAVE_FILE     = "query_index.xml";

/// @brief Index query time vs. number of records.
int main(int, char**) {
    perf::TimingsVector results;

    BOOST_FOREACH(size_t numData, perf::NUM_DATA) {
        perf::Timings t;
        t.addParam("numData", numData);
        t.addParam("length", LEN);

        cv::Mat data = perf::getRandomData(LEN, numData);
        vis::Index index;
        index.build("benchmark", data, TYPE);

        t.setTimings(perf::queryIndex(index, data));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

