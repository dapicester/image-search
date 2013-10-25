/**
 * @file demogui_slots.cpp
 * @brief Qt Gui for the image search demo, slots implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"
#include <QDebug>
#include <QProgressDialog>
#include <opencv2/core/core.hpp>

void
DemoGui::showAll() {
    // TODO
    qDebug() << "show all";
    messageBox("TODO");
}

void
DemoGui::search() {
    if (not loadIndex()) {
        messageBox("No index file found, please recompute index.", QMessageBox::Critical);
        return;
    }
    if (queryType != "color" and not loadVocabulary()) {
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Critical);
        return;
    }

    if (queryType != "color") Q_ASSERT(category == str(vocabulary->getCategory()));
    Q_ASSERT(category == str(index->getCategory()));
    Q_ASSERT(queryType == decodeType(index->getType()));
    qDebug() << "search " << category << " by " << queryType;

    cv::Mat query;
    if (realtimeCheckBox->isChecked()) {
        qDebug() << "computing descriptors ...";

        vis::Descriptors descriptors;
        extractDescriptors(category, queryType, PathList(1, queryImagePath), &descriptors, vocabulary.data());
        descriptors.get().copyTo(query);

        qDebug() << "descriptors computed";
    }
    else {
        if (not loadQueries()) {
            messageBox("No queries file found, please recompute queries.", QMessageBox::Critical);
            return;
        }
        Q_ASSERT(queryType == decodeType(queries->getType()));

        int queryId = queryList->currentRow();
        qDebug() << "query:" << queryId << queryNames[queryId];
        query = queries->get().col(queryId);
    }
    Q_ASSERT(query.cols == 1);

    std::vector<vis::Index::id_type> matches;
    index->query(query, matches, results.size());

    const PathList& images = imagesMap[category];
    for (int n = 0; n < results.size(); n++) {
        vis::Index::id_type i = matches[n];
        const fs::path& file = images[i];
        qDebug() << "(" << i << ")" << str(file);
        setImage(results[n], file);
    }

    qDebug() << "search done";
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

    descriptors.reset(new vis::Descriptors);
    extractDescriptors(category, queryType, names, descriptors.data(), vocabulary.data());

    fs::path savefile = descriptorsFile(DATA_PATH, category, queryType);
    descriptors->save(savefile);
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
    PathList names = ::queryNames(allnames, category);

    queries.reset(new vis::Descriptors);
    extractDescriptors(category, queryType, names, queries.data(), vocabulary.data());
    progress->setValue(6);

    fs::path savefile = queryFile(DATA_PATH, category, queryType);
    queries->save(savefile);
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

