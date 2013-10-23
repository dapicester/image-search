/**
 * @file vocabulary.cpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#include "vocabulary.hpp"
#include "callbacks.hpp"
#include "extract.hpp"
#include "kmeans.hpp"
#include "serialization.hpp"
#include <cstdio>

namespace vis {

using namespace cv;
using namespace std;

Vocabulary::Vocabulary() {}

Vocabulary::Vocabulary(const std::string c, const cv::Mat& data, size_t n)
    : category(c), numWords(n) {
    KMeans<float> kmeans;
    words = kmeans.cluster(data, numWords);

    kdtree.reset(new KDTree<float>(words));
}

Vocabulary::~Vocabulary() {}

Vocabulary*
Vocabulary::fromImageList(
        const string& category,
        const vector<fs::path>& names,
        size_t numWords) {
    const size_t len = names.size();
    const size_t numFeatures = cvRound(numWords*100.0/len);

    Mat descriptors;
    HogVocabularyCallback cb(numFeatures);
    extract(names, descriptors, cb);

    printf("Computing visual words and kdtree ...\n");
    Vocabulary* vocabulary = new Vocabulary(category, descriptors, numWords);

    return vocabulary;
}

Mat
Vocabulary::quantize(const Mat& descriptors) const {
    vector<KDTreeIndex> neighbors = kdtree->search<KDTreeIndex>(descriptors, 1, vocabulary::MAX_COMPARISONS);
    Mat quantized(descriptors.rows, descriptors.cols, descriptors.type());
    for (int i = 0; i < neighbors.size(); i++) {
        vl_uindex index = neighbors[i].index;
        words.col(index).copyTo(quantized.col(i));
    }
    // Mat::convertTo(cv::DataType<double>::type)
    return quantized;
}

Vocabulary*
Vocabulary::load(const fs::path& file) {
    printf("Loading vocabulary from file %s ... ", file.string().c_str());
    Vocabulary* vocabulary = vis::load<Vocabulary, BinarySerializer>(file);
    printf("done\n");
    return vocabulary;
}

void
Vocabulary::save(const fs::path& file) const {
    printf("Saving vocabulary to file %s ... ", file.string().c_str());
    vis::save<Vocabulary, BinarySerializer>(file, *this);
    printf("done\n");
}

} /* namespace vis */

