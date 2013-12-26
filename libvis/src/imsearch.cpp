/**
 * @file imsearch.cpp
 * @brief Façade class for the image search system.
 * @author Paolo D'Apice
 */

#include "vis/imsearch.hpp"

#include "vis/builder.hpp"
#include "vis/callbacks.hpp"
#include "vis/index.hpp"
#include "vis/vocabulary.hpp"
#include "vis/utils/filesystem.hpp"

#include <boost/assert.hpp>
#include <boost/filesystem.hpp>

namespace vis {

namespace fs = boost::filesystem;

ImageSearch::ImageSearch(const std::string& cat, DescriptorsType t,
        const fs::path& dir)
        : category(cat), type(t), dataDir(dir) {}

ImageSearch::~ImageSearch() {}

void
ImageSearch::load() {
    loadIndex();
    if (requiresVocabulary(type)) loadVocabulary();
}

void
ImageSearch::build() {
    if (requiresVocabulary(type)) buildVocabulary();
    buildDescriptors();
    buildIndex();
}

void
ImageSearch::save() const {
    saveIndex();
    if (requiresVocabulary(type)) saveVocabulary();
    saveDescriptors(); // XXX serve?
}

// FIXME back-compatibility with the gui demo
std::string typeString(DescriptorsType type) {
    switch (type) {
    case HOG:
        return "shape";
    case HSV:
        return "color";
    case HOG_HSV:
        return "combined";
    }
}

void
ImageSearch::loadIndex() {
    fs::path file = indexFile(dataDir, category, typeString(type));
    index.reset(Index::load(file));
    BOOST_ASSERT_MSG(index.get() != nullptr, "index is null");
}

void
ImageSearch::loadVocabulary() {
    fs::path file = vocabularyFile(dataDir, category);
    vocabulary.reset(Vocabulary::load(file));
    BOOST_ASSERT_MSG(vocabulary.get() != nullptr, "vocabulary is null");
}

void
ImageSearch::buildVocabulary() {
    Builder builder(dataDir, category, type);
    vocabulary.reset(builder.computeVocabulary());
}

void
ImageSearch::buildDescriptors() {
    Builder builder(dataDir, category, type);
    descriptors.reset(builder.computeDescriptors(vocabulary.get()));
}

void
ImageSearch::buildIndex() {
    Builder builder(dataDir, category, type);
    index.reset(builder.computeIndex(descriptors.get()));
}

void
ImageSearch::saveIndex() const {
    fs::path savefile = vis::indexFile(dataDir, category, typeString(type));
    index->save(savefile);
}

void
ImageSearch::saveDescriptors() const {
    fs::path savefile = vis::descriptorsFile(dataDir, category, typeString(type));
    descriptors->save(savefile);
}

void
ImageSearch::saveVocabulary() const {
    fs::path savefile = vis::vocabularyFile(dataDir, category);
    vocabulary->save(savefile);
}

void
ImageSearch::query(unsigned id, std::vector<size_t>& results, size_t numResults) const {
    index->query(arma::uvec{id}, results, numResults);
}

void
ImageSearch::query(const arma::fvec& descriptors, std::vector<size_t>& results, size_t numResults) const {
    index->query(descriptors, results, numResults);
}

void
ImageSearch::query(const cv::Mat& image, std::vector<size_t>& results, size_t numResults) const {
    index->query(extract(image), results, numResults);
}

arma::fvec
ImageSearch::extract(const cv::Mat& image) const {
    cv::Mat standardized = vis::standardizeImage(image);
    boost::scoped_ptr<Callback> callback(getCallback(type, requiresVocabulary(type) ? vocabulary.get() : nullptr));
    return (*callback)(standardized);
}

} // namespace vis

