/**
 * @file vocabulary.cpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#include "callbacks_hog.hpp"
#include "extract.hpp"
#include "kmeans.hpp"
#include "serialization.hpp"
#include "vocabulary.hpp"
#include <boost/assert.hpp>
#include <iostream>

namespace vis {

Vocabulary::Vocabulary(size_t nw) : numWords(nw) {}

void
Vocabulary::build(const std::string c, const arma::fmat& data) {
    category = c;

    KMeans<float> kmeans;
    words = kmeans.cluster(data, numWords);

    kdtree.reset(new KDTree<float>);
    kdtree->build(words);
}

Vocabulary::~Vocabulary() {}

Vocabulary*
Vocabulary::fromImageList(
        const std::string& category,
        const std::vector<fs::path>& names,
        size_t numWords) {
    const size_t len = names.size();
    const size_t numFeatures = cvRound(numWords*100.0/len);

    arma::fmat descriptors;
    HogVocabularyCallback cb(numFeatures);
    extract(names, descriptors, cb);

    std::cout << "Computing visual words and kdtree ..." << std::endl;
    Vocabulary* vocabulary = new Vocabulary(numWords);
    vocabulary->build(category, descriptors);

    return vocabulary;
}

arma::fmat
Vocabulary::quantize(const arma::fmat& descriptors) const {
    BOOST_ASSERT(descriptors.n_rows == words.n_rows);

    std::vector<KDTreeIndex> neighbors = kdtree->search<KDTreeIndex>(descriptors, 1, vocabulary::MAX_COMPARISONS);
    arma::fmat quantized(descriptors.n_rows, descriptors.n_cols);

    // FIXME should not return the quantized words but the indices!!!

    for (int i = 0; i < neighbors.size(); i++) {
        vl_uindex index = neighbors[i].index;
        quantized.col(i) = words.col(index);
    }

    return quantized;
}

Vocabulary*
Vocabulary::load(const fs::path& file) {
    std::cout << "Loading vocabulary from file " << file << " ... ";
    Vocabulary* vocabulary = vis::load<Vocabulary, BinarySerializer>(file);
    std::cout << "done" << std::endl;
    return vocabulary;
}

void
Vocabulary::save(const fs::path& file) const {
    std::cout << "Saving vocabulary to file " << file << " ... ";
    vis::save<Vocabulary, BinarySerializer>(file, *this);
    std::cout << "done" << std::endl;
}

} /* namespace vis */

