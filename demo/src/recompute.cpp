/**
 * @file recompute.cpp
 * @brief Utility for recomputing descriptors and indexes.
 * @author Paolo D'Apice
 */

//#include <vis.hpp>
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
Recompute::computeVocabulary(const QString& category) {
    qDebug() << "computing vocabulary for" << category;

    vector_path names = vis::subset(images, 100, vis::UNIFORM);
    vocabulary.reset(vis::Vocabulary::fromImageList(category.toStdString(), names));

    fs::path savefile = vocabularyFile(dataPath, category);
    vocabulary->save(savefile);

    qDebug() << "vocabulary computed";
}

void
Recompute::computeDescriptors(const QString& category, const QString& type) {
    qDebug() << "computing descriptors for" << category << "with" << type;

    descriptors.reset(new vis::Descriptors);
    // XXX quick'n dirty (TM)
    if (type == "color") {
        vis::HsvHistogramsCallback cb;
        descriptors->compute(category.toStdString(), images, cb);
    }
    else if (type == "shape") {
        vis::HogBagOfWordsCallback cb(vocabulary.data());
        descriptors->compute(category.toStdString(), images, cb);
    }
    else if (type == "combined") {
        vis::CompositeCallback cb(vocabulary.data());
        descriptors->compute(category.toStdString(), images, cb);
    }

    fs::path savefile = descriptorsFile(dataPath, category, type);
    descriptors->save(savefile);

    qDebug() << "descriptors computed";
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
    if (type != "color") computeVocabulary(category); // color histograms do not need vocabulary
    computeDescriptors(category, type);
    computeIndex(category, type);

    qDebug() << "recomputing done";
}

