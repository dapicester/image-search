/**
 * @file perf_index_build.cpp
 * @brief Simple benchmark on index building.
 * @author Paolo D'Apice
 */

#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"

using namespace perf;

static const vis::DescriptorsType TYPE = vis::HOG_HSV;
static const size_t LEN = getLength(TYPE);
static const std::string SAVE_FILE = "build_index.xml";

/// @brief Index build time vs. number of records.
int main(int, char**) {
    IndexTimingsVector results;

    BOOST_FOREACH(size_t size, SIZES) {
        IndexTimings t;
        t.addParam("size", size);

        cv::Mat data = getRandomData(LEN, size);
        t.setTimings(perfBuildIndex(data, TYPE));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

