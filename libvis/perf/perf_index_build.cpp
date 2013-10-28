/**
 * @file perf_index_build.cpp
 * @brief Simple benchmark on index building performance.
 * @author Paolo D'Apice
 */

#include "utils/data.hpp"
#include "perf_data.hpp"
#include "perf_index.hpp"
#include "perf_utils.hpp"
#include <cstdio>

using namespace perf;

int main(int, char**) {
    IndexTimingsVector results;
    results.resize(SIZES.size());

    for(int i = 0; i < SIZES.size(); i++) {
        IndexTimings t;
        t.size = SIZES[i];
        t.type = vis::HOG_HSV;

        size_t length = getLength(t.type);

        // TODO use opencv randu
        float* ptr = test::getTestDataPtr<float>(t.size, length);
        cv::Mat data(length, t.size, cv::DataType<float>::type, ptr);

        t.timings = perfBuildIndex(data, t.type);

        results[i] = t;
        delete[] ptr;
    }

    perf::save("build_index.xml", results);
}
