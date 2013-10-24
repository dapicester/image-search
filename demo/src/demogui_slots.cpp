/**
 * @file demogui_slots.cpp
 * @brief Qt Gui for the image search demo, slots implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"
#include <QDebug>
#include <QProgressDialog>

void
DemoGui::showAll() {
    // TODO
    qDebug() << "show all";
    messageBox("TODO");
}

void
DemoGui::search() {
    // TODO
    qDebug() << "search";
    messageBox("TODO");
}

void
DemoGui::recomputeIndex() {
    if (not confirmMessageBox("Recompute index")) {
        qDebug() << "canceled";
        return;
    }

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing index ...", this, 1, 10));

    if (not loadDescriptors()) {
        messageBox("No descriptors file found, please recompute descriptors.", QMessageBox::Critical);
        return;
    }
    progress->setValue(3);

    index.reset(new vis::Index);
    index->build(category.toStdString(), *descriptors);
    progress->setValue(6);

    fs::path savefile = indexFile(DATA_PATH, category, queryType);
    index->save(savefile);
    progress->setValue(10);

    qDebug() << "index done!";
}

void
DemoGui::recomputeDescriptors() {
    if (not confirmMessageBox("Recompute descriptors")) {
        qDebug() << "canceled";
        return;
    }

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing descriptors ...", this, 1, 10));

    if (not loadVocabulary()) {
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Critical);
        return;
    }
    progress->setValue(3);

    const PathList& names = imagesMap[category];
    progress->setValue(6);

    vis::Descriptors descriptors;
    // XXX quick'n dirty (TM)
    if (queryType == "color") {
        vis::HsvHistogramsCallback cb;
        descriptors.compute(category.toStdString(), names, cb);
    }
    else if (queryType == "shape") {
        vis::HogBagOfWordsCallback cb(vocabulary.data());
        descriptors.compute(category.toStdString(), names, cb);
    }
    else if (queryType == "combined") {
        vis::CompositeCallback cb(vocabulary.data());
        descriptors.compute(category.toStdString(), names, cb);
    }

    fs::path savefile = descriptorsFile(DATA_PATH, category, queryType);
    descriptors.save(savefile);
    progress->setValue(10);

    qDebug() << "descriptors done!";
}

void
DemoGui::recomputeQueries() {
    if (not confirmMessageBox("Recompute queries")) {
        qDebug() << "canceled";
        return;
    }

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing queries ...", this, 1, 10));

    if (not loadVocabulary()) {
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Critical);
        return;
    }
    progress->setValue(3);

    static fs::path file = categoryFile(DATA_PATH, "test");
    static fs::path dir = categoryDir(DATA_PATH, "test");
    static PathList allnames = loadNames(file, dir);
    PathList names;
    QRegExp re("\\b" + category + "\\d");
    for (PathList::iterator it = allnames.begin(); it != allnames.end(); ++it) {
        qDebug() << it->c_str();
        if (str(*it).contains(re)) {
            names.push_back(*it);
            qDebug() << "pushed";
        }
    }
    progress->setValue(6);

    vis::Descriptors queries;
    // XXX quick'n dirty (TM)
    if (queryType == "color") {
        vis::HsvHistogramsCallback cb;
        queries.compute("test", names, cb);
    }
    else if (queryType == "shape") {
        vis::HogBagOfWordsCallback cb(vocabulary.data());
        queries.compute("test", names, cb);
    }
    else if (queryType == "combined") {
        vis::CompositeCallback cb(vocabulary.data());
        queries.compute("test", names, cb);
    }

    fs::path savefile = descriptorsFile(DATA_PATH, "test", queryType);
    queries.save(savefile);
    progress->setValue(10);

    qDebug() << "queries done!";
}

void
DemoGui::recomputeVocabulary() {
    if (not confirmMessageBox("Recompute vocabulary")) {
        qDebug() << "canceled";
        return;
    }

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing vocabulary ...", this, 0, 10));
    progress->setValue(3);

    PathList names = vis::subset(imagesMap[category], 100, vis::UNIFORM);
    vocabulary.reset(vis::Vocabulary::fromImageList(category.toStdString(), names));
    progress->setValue(6);

    fs::path savefile = vocabularyFile(DATA_PATH, category);
    vocabulary->save(savefile);
    progress->setValue(10);

    qDebug() << "vocabulary done!";
}

