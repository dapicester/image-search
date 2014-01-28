/**
 * @file recompute.cpp
 * @brief Utility for recomputing descriptors and indexes.
 * @author Paolo D'Apice
 */

#include "recompute.hpp"
#include "utils.hpp"

#include <vis.hpp>
#include <vis/utils/filesystem.hpp>
#include <vis/utils/handlers.hpp>

#include <iostream>

Recompute::Recompute(const fs::path& path) : dataPath(path) {}

Recompute::~Recompute() {}

void
Recompute::loadImagePaths(const std::string& category) {
    std::cout << "loading images for " << category << std::cout;

    fs::path file = vis::categoryFile(dataPath, category);
    fs::path dir = dataPath / category;
    images = vis::loadNames(file, dir);

    std::cout << "loaded " << images.size() << " images for " << category << std::cout;
}

void
Recompute::loadQueryImagePaths(const std::string& category) {
    std::cout << "loading images for " << category << std::cout;

    static fs::path file = vis::categoryFile(dataPath, "test");
    static fs::path dir = dataPath / "test";
    static vector_path names = vis::loadNames(file, dir);
    queryImages = queryNames(names, str(category));

    std::cout << "loaded " << queryImages.size() << " query images for " << category;
}

#define PROGRESS_HANDLER(files) [&](int i) { vis::handlers::PrintFile(i, files); }

void
Recompute::computeVocabulary(const std::string& category) {
    std::cout << "computing vocabulary for " << category;

    vector_path names = vis::subset(images, 100);
    vocabulary.reset(vis::Vocabulary::fromImageList<vis::HogExtractor>(
                category, names, vis::vocabulary::NUM_WORDS,
                PROGRESS_HANDLER(names)));

    fs::path savefile = vis::vocabularyFile(dataPath, category);
    vocabulary->save(savefile);

    std::cout << "vocabulary computed" << std::cout;
}

void
Recompute::computeDescriptors(const std::string& category, const std::string& type) {
    std::cout << "computing descriptors for " << category << " with " << type << std::cout;

    descriptors.reset(new vis::Descriptors);
    extractDescriptors(str(category), str(type), images,
            descriptors.get(), vocabulary.get(),
            PROGRESS_HANDLER(images));

    fs::path savefile = vis::descriptorsFile(dataPath, category, type);
    descriptors->save(savefile);

    std::cout << "descriptors computed " << std::cout;
}

void
Recompute::computeQueries(const std::string& category, const std::string& type) {
    std::cout << "computing query descriptors for " << category << " with " << type << std::cout;

    queries.reset(new vis::Descriptors);
    extractDescriptors(str(category), str(type), queryImages,
            queries.get(), vocabulary.get(),
            PROGRESS_HANDLER(queryImages));

    fs::path savefile = queryFile(dataPath, str(category), str(type));
    queries->save(savefile);

    std::cout << "query descriptors computed " << std::cout;
}

void
Recompute::computeIndex(const std::string& category, const std::string& type) {
    std::cout << "computing index for " << category << " with " << type << std::cout;

    index.reset(new vis::Index);
    index->build(category, *descriptors);

    fs::path savefile = vis::indexFile(dataPath, category, type);
    index->save(savefile);

    std::cout << "index computed" << std::cout;
}

void
Recompute::recompute(const std::string& category, const std::string& type) {
    std::cout << "recomputing all for " << category << " with " << type << std::cout;

    loadImagePaths(category);
    loadQueryImagePaths(category);

    if (type != "color") computeVocabulary(category); // color histograms do not need vocabulary
    computeDescriptors(category, type);
    computeQueries(category, type);
    computeIndex(category, type);

    std::cout << "recomputing done" << std::cout;
}

