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
}

void
DemoGui::search() {
    // TODO
    qDebug() << "search";
}

void
DemoGui::recomputeIndex() {
    if (not confirmMessageBox("Recompute descriptors")) {
        qDebug() << "canceled";
        return;
    }

    fs::path file = categoryFile(DATA_PATH, category);
    fs::path dir = categoryDir(DATA_PATH, category);

    // TODO load descriptors matrix, build index;
    //index.reset(new vis::Index);

    //fs::path savefile = indexFile(DATA_PATH, category, queryType);
    //index->save(savefile);

    qDebug() << "vocabulary done!";
}

void
DemoGui::recomputeDescriptors() {
    if (not confirmMessageBox("Recompute descriptors")) {
        qDebug() << "canceled";
        return;
    }

    QProgressDialog* progress = progressDialog("Computing descriptors ...", this, 1, 10);

    if (!vocabulary) {
        loadVocabulary();
        progress->setValue(3);
    }

    fs::path file = categoryFile(DATA_PATH, category);
    fs::path dir = categoryDir(DATA_PATH, category);
    std::vector<fs::path> names = loadNames(file, dir);
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
    delete progress;
}

void
DemoGui::recomputeVocabulary() {
    if (not confirmMessageBox("Recompute vocabulary")) {
        qDebug() << "canceled";
        return;
    }

    QProgressDialog* progress = progressDialog("Computing vocabulary ...", this, 0, 10);
    progress->setValue(3);

    fs::path file = categoryFile(DATA_PATH, category);
    fs::path dir = categoryDir(DATA_PATH, category);
    std::vector<fs::path> names = loadNames(file, dir);
    names = vis::subset(names, 100, vis::UNIFORM);
    vocabulary.reset(vis::Vocabulary::fromImageList(category.toStdString(), names));
    progress->setValue(6);

    fs::path savefile = vocabularyFile(DATA_PATH, category);
    vocabulary->save(savefile);
    progress->setValue(10);

    qDebug() << "vocabulary done!";
    delete progress;
}

