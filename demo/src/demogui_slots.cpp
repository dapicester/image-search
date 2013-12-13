/**
 * @file demogui_slots.cpp
 * @brief Qt Gui for the image search demo, slots implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"
#include <vis/utils/filesystem.hpp>

#include <QDebug>
#include <QProgressDialog>

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

    arma::fmat query;
    if (realtimeCheckBox->isChecked()) {
        qDebug() << "computing descriptors ...";

        vis::Descriptors descriptors;
        extractDescriptors(category, queryType, PathList(1, queryImagePath), &descriptors, vocabulary.data());
        query = descriptors.data();

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
        try {
            query = queries->data().col(queryId);
        } catch (...) {
            messageBox("Query file found, please recompute queries.", QMessageBox::Critical);
            return;
        }
    }
    Q_ASSERT(query.n_cols == 1);

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

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing index ...", this, 3));

    if (not loadDescriptors()) {
        messageBox("No descriptors file found, please recompute descriptors.", QMessageBox::Critical);
        return;
    }
    progress->setValue(1);

    index.reset(new vis::Index);
    index->build(category.toStdString(), *descriptors);
    progress->setValue(2);

    fs::path savefile = vis::indexFile(DATA_PATH, category.toStdString(), queryType.toStdString());
    index->save(savefile);
    progress->setValue(3);

    qDebug() << "index done!";
}

void
DemoGui::recomputeDescriptors() {
    if (not confirmMessageBox("Recompute descriptors")) {
        qDebug() << "canceled";
        return;
    }

    const PathList& names = imagesMap[category];

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing descriptors ...", this, names.size() + 3));

    if (not loadVocabulary()) {
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Critical);
        return;
    }
    progress->setValue(1);

    descriptors.reset(new vis::Descriptors);
    extractDescriptors(category, queryType, names,
            descriptors.data(), vocabulary.data(), [&](int i) {
                progress->setValue(i+2);
                qDebug() << "processing file" << str(names[i]);
            });

    fs::path savefile = vis::descriptorsFile(DATA_PATH, category.toStdString(), queryType.toStdString());
    descriptors->save(savefile);
    progress->setValue(names.size() + 3);

    qDebug() << "descriptors done!";
}

void
DemoGui::recomputeQueries() {
    if (not confirmMessageBox("Recompute queries")) {
        qDebug() << "canceled";
        return;
    }

    static fs::path file = vis::categoryFile(DATA_PATH, "test");
    static fs::path dir = DATA_PATH / "test";
    static PathList allnames = vis::loadNames(file, dir);
    PathList names = ::queryNames(allnames, category);

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing queries ...", this, names.size() + 3));

    if (not loadVocabulary()) {
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Critical);
        return;
    }
    progress->setValue(1);

    queries.reset(new vis::Descriptors);
    extractDescriptors(category, queryType, names,
            queries.data(), vocabulary.data(), [&](int i) {
                progress->setValue(i+2);
                qDebug() << "processing file" << str(names[i]);
            });

    fs::path savefile = queryFile(DATA_PATH, category, queryType);
    queries->save(savefile);
    progress->setValue(names.size());

    qDebug() << "queries done!";
}

void
DemoGui::recomputeVocabulary() {
    if (not confirmMessageBox("Recompute vocabulary")) {
        qDebug() << "canceled";
        return;
    }

    PathList names = vis::subset(imagesMap[category], 100);

    QScopedPointer<QProgressDialog> progress(progressDialog("Computing vocabulary ...", this, names.size() + 3));
    progress->setValue(1);

    vocabulary.reset(vis::Vocabulary::fromImageList<vis::HogExtractor>(
                category.toStdString(), names, vis::vocabulary::NUM_WORDS, [&](int i) {
                    progress->setValue(i+2);
                    qDebug() << "processing file" << str(names[i]);
                }));

    fs::path savefile = vis::vocabularyFile(DATA_PATH, category.toStdString());
    vocabulary->save(savefile);
    progress->setValue(names.size());

    qDebug() << "vocabulary done!";
}

