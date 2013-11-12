/**
 * @file vocabulary.cpp
 * @brief Visual word vocabulary
 * @author Paolo D'Apice
 */

#include "kmeans.hpp"
#include "serialization.hpp"
#include "vocabulary.hpp"
#include <boost/assert.hpp>
#include <iostream>

namespace vis {

static const vl_size MAX_COMPARISONS = 15;

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

arma::fmat
Vocabulary::quantize(const arma::fmat& descriptors) const {
    BOOST_ASSERT(descriptors.n_rows == words.n_rows);

    std::vector<KDTreeIndex> neighbors = kdtree->search<KDTreeIndex>(descriptors, 1, MAX_COMPARISONS);
    arma::fmat quantized(descriptors.n_rows, descriptors.n_cols);

    for (int i = 0; i < neighbors.size(); i++) {
        vl_uindex index = neighbors[i].index;
        quantized.col(i) = words.col(index);
    }

    return quantized;
}

arma::uvec
Vocabulary::lookup(const arma::fmat& descriptors) const {
    BOOST_ASSERT(descriptors.n_rows == words.n_rows);

    std::vector<KDTreeIndex> neighbors = kdtree->search<KDTreeIndex>(descriptors, 1, MAX_COMPARISONS);
    arma::uvec indices(descriptors.n_cols);

    for (int i = 0; i < neighbors.size(); i++) {
        indices(i) = neighbors[i].index;
    }

    return indices;
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

