/**
 * @file perf_descriptors.cpp
 * @brief Simple benchmark on descriptors computation.
 * @author Paolo D'Apice
 */

#include "dir.h"
#include "vis.hpp"

#include "perf_data.hpp"
#include "perf_utils.hpp"
#include <boost/array.hpp>
#include <boost/foreach.hpp>
#include <boost/scoped_ptr.hpp>

/// Number of times a single test is executed.
static const int NUM_EXECUTIONS = 10;

/// Available descriptors.
static const boost::array<vis::DescriptorsType,3> TYPES = { vis::HSV, vis::HOG, vis::HOG_HSV };

static const std::string SAVE_FILE = "descriptors.xml";

namespace fs = boost::filesystem;

/// Load vocabulary from file.
vis::Vocabulary* loadVocabulary();

/// Get files.
std::vector<fs::path> getFiles();

/// Compute descriptors.
void extractDescriptors(vis::DescriptorsType, const std::vector<fs::path>&, const vis::Vocabulary*);

/// idem
std::vector<timestamp_t> computeDescriptors(vis::DescriptorsType, std::vector<fs::path>&, const vis::Vocabulary*);

/// @brief Process time vs. descriptor type.
int main(int, char**) {
    boost::scoped_ptr<vis::Vocabulary> vocabulary( loadVocabulary() );
    std::vector<fs::path> files = getFiles();

    perf::TimingsVector results;

    BOOST_FOREACH(vis::DescriptorsType type, TYPES) {
        size_t length = perf::getLength(type);

        perf::Timings t;
        t.addParam("descriptor", perf::getName(type));
        t.addParam("length", length);

        t.setTimings(computeDescriptors(type, files, vocabulary.get()));

        results.push_back(t);
    }

    perf::save(SAVE_FILE, results);
}

vis::Vocabulary* loadVocabulary() {
    using boost::filesystem::path;

    static const path vocabularyFile = path(DATA_DIR) / "vocabulary_bag.dgz";
    BOOST_ASSERT_MSG(fs::is_regular_file(vocabularyFile), "Cannot find vocabulary file");

    vis::Vocabulary* vp = vis::Vocabulary::load(vocabularyFile);
    BOOST_ASSERT_MSG(vp, "Cannot load vocabulary from file");

    return vp;
};

std::vector<fs::path> getFiles() {
    boost::array<std::string,4> categories = { "bag", "shoe", "woman_shoe", "test" };
    std::vector<fs::path> files;
    BOOST_FOREACH(const std::string& category, categories) {
        fs::path dir = fs::path(DATA_DIR) / category;
        std::vector<fs::path> images = vis::getImageFiles(dir);
        std::copy(images.begin(), images.end(), std::back_inserter(files));
    }
    return files;
}

inline void extractDescriptors(vis::DescriptorsType type,
        const std::vector<fs::path>& files, const vis::Vocabulary* vocabulary) {
    vis::Descriptors descriptors;
    switch (type) {
        case vis::HSV:
            {
            vis::HsvHistogramsCallback cb;
            descriptors.compute("benchmark", files, cb);
            }
            break;
        case vis::HOG:
            {
            vis::HogBagOfWordsCallback cb(vocabulary);
            descriptors.compute("benchmark", files, cb);
            }
            break;
        case vis::HOG_HSV:
            {
            vis::CompositeCallback cb(vocabulary);
            descriptors.compute("benchmark", files, cb);
            }
            break;
    }
}

std::vector<timestamp_t> computeDescriptors(vis::DescriptorsType type,
        std::vector<fs::path>& files, const vis::Vocabulary* vocabulary) {
    PosixTimer timer;
    std::vector<timestamp_t> timings;

    for (int i = 0; i < NUM_EXECUTIONS; i++) {
        std::random_shuffle(files.begin(), files.end());
        std::vector<fs::path> names = vis::subset(files, 100, vis::UNIFORM);

        timer.tic();
        extractDescriptors(type, names, vocabulary);
        timer.toc();

        timestamp_t elapsed = timer.getMillis();
        printf("Process time (%d): %llu ms\n", i, elapsed);
        timings.push_back(elapsed);
    }

    return timings;
}

