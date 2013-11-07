/**
 * @file perf_index.cpp
 * @brief Performance tests on index.
 * @author Paolo D'Apice
 */

#include "perf_index.hpp"

namespace perf {

std::vector<Timer::timestamp_t>
buildIndex(const cv::Mat& data, vis::DescriptorsType type, size_t numTrees) {
    Timer timer;
    std::vector<Timer::timestamp_t> timings;

    for (int i = 0; i < NUM_EXECUTIONS; i++) {
        timer.tic();
        vis::Index index;
        index.build("benchmark", data, type, numTrees);
        timer.toc();

        Timer::timestamp_t elapsed = timer.getMillis();
        printf("Build time (%d): %llu ms\n", i, elapsed);
        timings.push_back(elapsed);
    }

    return timings;
}

std::vector<Timer::timestamp_t>
queryIndex(const vis::Index& index, const cv::Mat& data,
        size_t neighbors, size_t maxComparisons) {
    Timer timer;
    std::vector<Timer::timestamp_t> timings;

    for (int i = 0; i < NUM_EXECUTIONS; i++) {
        // query = random select one row
        int idx = rand() % data.cols;
        cv::Mat query = data.col(idx);

        timer.tic();
        std::vector<vis::Index::id_type> matches;
        index.query(query, matches, neighbors, maxComparisons);
        timer.toc();

        Timer::timestamp_t elapsed = timer.getMillis();
        printf("Query time (%d): %llu ms\n", i, elapsed);
        timings.push_back(elapsed);
    }

    return timings;
}


} /* namespace perf */

