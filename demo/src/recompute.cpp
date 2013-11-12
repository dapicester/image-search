/**
 * @file recompute.cpp
 * @brief Utility for recomputing descriptors and indexes.
 * @author Paolo D'Apice
 */

#include "recompute.hpp"
#include "utils.hpp"
#include <vis.hpp>
#include <QDebug>

Recompute::Recompute(const char* path) : dataPath(path) {}

Recompute::Recompute(const fs::path& path) : dataPath(path) {}

Recompute::~Recompute() {}

void
Recompute::loadImagePaths(const QString& category) {
    qDebug() << "loading images for" << category;

    fs::path file = categoryFile(dataPath, category);
    fs::path dir = categoryDir(dataPath, category);
    images = loadNames(file, dir);

    qDebug() << "loaded" << images.size() << "images for" << category;
}

void
Recompute::loadQueryImagePaths(const QString& category) {
    qDebug() << "loading images for" << category;

    static fs::path file = categoryFile(dataPath, "test");
    static fs::path dir = categoryDir(dataPath, "test");
    static vector_path names = loadNames(file, dir);
    queryImages = queryNames(names, category);

    qDebug() << "loaded" << queryImages.size() << "query images for" << category;
}

void
Recompute::computeVocabulary(const QString& category) {
    qDebug() << "computing vocabulary for" << category;

    vector_path names = vis::subset(images, 100);
    vocabulary.reset(vis::Vocabulary::fromImageList<vis::HogExtractor>(category.toStdString(), names));

    fs::path savefile = vocabularyFile(dataPath, category);
    vocabulary->save(savefile);

    qDebug() << "vocabulary computed";
}

void
Recompute::computeDescriptors(const QString& category, const QString& type) {
    qDebug() << "computing descriptors for" << category << "with" << type;

    descriptors.reset(new vis::Descriptors);
    extractDescriptors(category, type, images, descriptors.data(), vocabulary.data());

    fs::path savefile = descriptorsFile(dataPath, category, type);
    descriptors->save(savefile);

    qDebug() << "descriptors computed";
}

void
Recompute::computeQueries(const QString& category, const QString& type) {
    qDebug() << "computing query descriptors for" << category << "with" << type;

    queries.reset(new vis::Descriptors);
    extractDescriptors(category, type, queryImages, queries.data(), vocabulary.data());

    fs::path savefile = queryFile(dataPath, category, type);
    queries->save(savefile);

    qDebug() << "query descriptors computed";
}

void
Recompute::computeIndex(const QString& category, const QString& type) {
    qDebug() << "computing index for" << category << "with" << type;

    index.reset(new vis::Index);
    index->build(category.toStdString(), *descriptors);

    fs::path savefile = indexFile(dataPath, category, type);
    index->save(savefile);

    qDebug() << "index computed";
}

void
Recompute::recompute(const QString& category, const QString& type) {
    qDebug() << "recomputing all for" << category << "with" << type;

    loadImagePaths(category);
    loadQueryImagePaths(category);

    if (type != "color") computeVocabulary(category); // color histograms do not need vocabulary
    computeDescriptors(category, type);
    computeQueries(category, type);
    computeIndex(category, type);

    qDebug() << "recomputing done";
}

