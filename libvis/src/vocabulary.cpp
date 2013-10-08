/**
 * @file vocabulary.cpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#include "vocabulary.hpp"
#include "hog.hpp"
#include "kmeans.hpp"
#include "standardize.hpp"
#include "serialization.hpp"
#include "utils.hpp"
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
        const vector<string>& names,
        size_t numWords) {
    const size_t len = names.size();
    const size_t numFeatures = iround(numWords*100.0/len);

    HogExtractor hog;
    Mat descriptors;

    // TODO openmp parfor
    size_t i = 0;
    for (vector<string>::const_iterator it = names.begin(); it != names.end(); ++it) {
        const string& name = *it;
        printf("  Extracting features from %s (%lu/%lu)\n", name.c_str(), i+1, len);

        Mat input = imread(name, CV_LOAD_IMAGE_GRAYSCALE); // hog needs only grayscale
        Mat image; // TODO change standardize signature to allow return?
        standardizeImage(input, image);

        Mat d = hog.extract(image).toMat();
        d = colsubset<float>(d, numFeatures, UNIFORM);
        d = d.t();

        descriptors.push_back(d);
        ++i;
    }
    descriptors = descriptors.t();

    printf("Computing visual words and kdtree ...\n");
    Vocabulary* vocabulary = new Vocabulary(category, descriptors, numWords);

    return vocabulary;
}

Vocabulary*
Vocabulary::fromImageList(
        const string& category,
        const vector<fs::path>& names,
        size_t numWords) {
    vector<string> files;
    path2string(names, files);
    return fromImageList(category, files, numWords);
}

Vocabulary*
Vocabulary::load(const fs::path& file) {
    printf("Loading vocabulary from file %s ... ", file.string().c_str());
    BinarySerializer<Vocabulary>::Loader loader;
    Vocabulary* vocabulary = new Vocabulary;
    loader(file.string().c_str(), *vocabulary);
    printf("done\n");
    return vocabulary;
}

void
Vocabulary::save(const fs::path& file) const {
    printf("Saving vocabulary to file %s ... ", file.string().c_str());
    BinarySerializer<Vocabulary>::Saver saver;
    saver(file.string().c_str(), *this);
    printf("done\n");
}

} /* namespace vis */

