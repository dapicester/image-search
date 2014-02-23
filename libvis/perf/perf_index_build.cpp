/**
 * @file perf_index_build.cpp
 * @brief Simple benchmark on index building.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN                = vis::getLength(TYPE);

static const std::string SAVE_FILE     = "build_index.xml";

/// @brief Index build time vs. number of records.
int main(int, char**) {
    perf::TimingsVector results;

    for (size_t numData : perf::NUM_DATA) {
        perf::Timings t;
        t.addParam("numData", numData);
        t.addParam("length", LEN);

        arma::fmat data = perf::getRandomData(LEN, numData);

        t.setTimings(perf::buildIndex(data, TYPE));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

