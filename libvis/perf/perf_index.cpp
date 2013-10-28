/**
 * @file perf_index.cpp
 * @brief Performance tests on index.
 * @author Paolo D'Apice
 */

#include "perf_index.hpp"

namespace perf {

std::vector<timestamp_t>
perfBuildIndex(const cv::Mat& data, vis::DescriptorsType type) {
    PosixTimer timer;
    std::vector<timestamp_t> timings;

    for (int i = 0; i < NUM_EXECUTIONS; i++) {
        timer.tic();
        vis::Index index;
        index.build("benchmark", data, type);
        timer.toc();

        timestamp_t elapsed = timer.getMillis();
        printf("Build time (%d): %llu ms\n", i, elapsed);
        timings.push_back(elapsed);
    }

    return timings;
}

std::vector<timestamp_t>
perfQueryIndex(const vis::Index& index, const cv::Mat& data, size_t neighbors) {
    srand(time(NULL));
    PosixTimer timer;
    std::vector<timestamp_t> timings;

    for (int i = 0; i < NUM_EXECUTIONS; i++) {
        // query = random select one row
        int idx = rand() % data.cols;
        cv::Mat query = data.col(idx);

        timer.tic();
        std::vector<vis::Index::id_type> matches;
        index.query(query, matches, neighbors);
        timer.toc();

        timestamp_t elapsed = timer.getMillis();
        printf("Query time (%d): %llu ms\n", i, elapsed);
        timings.push_back(elapsed);
    }

    return timings;
}


} /* namespace perf */

