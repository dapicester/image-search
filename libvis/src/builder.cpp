/**
 * @file builder.cpp
 * @brief Build descriptors and indexes.
 * @author Paolo D'Apice
 */

#include "vis/builder.hpp"

#include "vis/callbacks.hpp"
#include "vis/descriptors.hpp"
#include "vis/hog.hpp"
#include "vis/index.hpp"
#include "vis/vocabulary.hpp"
#include "vis/utils/filesystem.hpp"
#include "vis/utils/handlers.hpp"

namespace vis {

namespace fs = boost::filesystem;

Builder::Builder(const fs::path& path, const std::string& cat, DescriptorsType tp)
        : dataPath(path), category(cat), type(tp) {}

Builder::~Builder() {}

void
Builder::loadImageNames() {
    fs::path file = categoryFile(dataPath, category);
    fs::path dir = dataPath / category;
    images = loadNames(file, dir);
}

#define PROGRESS_HANDLER(files) [&](int i) { vis::handlers::PrintFile(i, files); }

Vocabulary*
Builder::computeVocabulary() {
    if (images.empty()) loadImageNames();

    vector_path names = vis::subset(images, 100); // XXX magic number!
    return Vocabulary::fromImageList<HogExtractor>(
                category, names, vocabulary::NUM_WORDS,
                PROGRESS_HANDLER(names));
}

Descriptors*
Builder::computeDescriptors(const Vocabulary* vocabulary) {
    if (images.empty()) loadImageNames();

    boost::scoped_ptr<Callback> callback(getCallback(type, requiresVocabulary(type) ? vocabulary : nullptr));
    ColorMode colorMode = type == HOG ? ColorMode::GRAYSCALE : ColorMode::COLORS; // TODO hide colormode

    Descriptors* descriptors = new vis::Descriptors;
    descriptors->compute(category, images, *callback, colorMode, PROGRESS_HANDLER(images));

    return descriptors;
}

Index*
Builder::computeIndex(const Descriptors* descriptors) {
    Index* index = new vis::Index;
    index->build(category, *descriptors);

    return index;
}

} // namespace vis

