/**
 * @file vocabulary.cpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#include "vocabulary.hpp"
#include "hog.hpp"
#include "utils.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

namespace vis {

using namespace cv;
using namespace std;

Vocabulary::Vocabulary(const std::string c, const cv::Mat& d, size_t n)
    : category(c), data(d), numWords(n) {
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

    vector<Mat> descriptors(len);
    HogExtractor hog;

    // TODO openmp parfor
    size_t i = 0;
    for(vector<string>::const_iterator it = names.begin(); it != names.end(); ++it) {
        const string& name = *it;
        cout << "  Extracting features from " << name
             << "(" << i << "/" << len << ")" << endl;

        Mat input = imread(name, CV_LOAD_IMAGE_GRAYSCALE); // hog needs only grayscale
        Mat image; // TODO change standardize signature to allow return?
        standardizeImage(input, image);

        Mat d = hog.extract(image).toMat();
        descriptors[i] = colsubset<float>(d, numFeatures, UNIFORM);

        ++i;
    }

    // TODO [descriptors{:}]
    Mat descriptors_array;
    vector<Mat>::const_iterator it = descriptors.begin(), end = descriptors.end();
    for (; it != end; ++it) {
        descriptors_array.push_back(*it);
    }
    descriptors_array = descriptors_array.reshape(0, 1);

    cout << "Computing visual words and kdtree ..." << endl;
    Vocabulary* vocabulary = new Vocabulary(category, descriptors_array, numWords);

    return vocabulary;
}

} /* namespace vis */

