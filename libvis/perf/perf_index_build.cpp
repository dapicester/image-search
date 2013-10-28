/**
 * @file perf_index_build.cpp
 * @brief Simple benchmark on index building performance.
 * @author Paolo D'Apice
 */

#include "vis.hpp"
#include "utils/data.hpp"
#include "utils/posixtimer.hpp"
#include "serialization.hpp"
#include <boost/array.hpp>
#include <cstdio>

static const int NUM_EXECUTIONS = 10;
static const boost::array<size_t,3> SIZES = { 1e4, 1e5, 1e6 };
static const boost::array<vis::DescriptorsType,3> TYPES = { vis::HSV, vis::HOG, vis::HOG_HSV };

// TODO move this to descriptors_type.hpp
size_t getLength(vis::DescriptorsType type) {
    switch(type) {
        case vis::HSV: return 166;
        case vis::HOG: return 300;
        case vis::HOG_HSV: return 466;
    }
}

struct BuildIndexTimes {
    size_t size;
    vis::DescriptorsType type;
    std::vector<timestamp_t> times;

private:
    friend class boost::serialization::access;

    template <typename Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::make_nvp("size",size);
        ar & boost::serialization::make_nvp("type", type);
        ar & boost::serialization::make_nvp("times", times);
    }
};

std::vector<timestamp_t>
perfBuildIndex(const cv::Mat& data, vis::DescriptorsType type) {
    PosixTimer timer;
    std::vector<timestamp_t> times;

    for (int i = 0; i < NUM_EXECUTIONS; i++) {
        timer.tic();
        vis::Index index;
        index.build("benchmark", data, type);
        timer.toc();

        timestamp_t elapsed = timer.getMillis();
        printf("Build time (%d): %llu ms\n", i, elapsed);
        times.push_back(elapsed);
    }

    return times;
}

int main(int, char**) {
    std::vector< std::vector<BuildIndexTimes> > results;
    results.resize(SIZES.size());
    for (int i = 0; i < SIZES.size(); i++)
        results[i].resize(TYPES.size());

    for (int i = 0; i < SIZES.size(); i++) {
        size_t size = SIZES[i];

        for(int j = 0; j < TYPES.size(); j++) {
            vis::DescriptorsType type = TYPES[j];

            BuildIndexTimes b;
            b.size = size;
            b.type = type;

            size_t length = getLength(type);

            float* ptr = test::getTestDataPtr<float>(size, length);
            cv::Mat data(size, length, cv::DataType<float>::type, ptr);

            b.times = perfBuildIndex(data, type);

            results[i][j] = b;
            delete[] ptr;
        }
    }

    perf::save("build_index.xml", results);
}
