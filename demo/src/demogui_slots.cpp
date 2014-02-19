/**
 * @file demogui_slots.cpp
 * @brief Qt Gui for the image search demo, slots implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"

#include <vis/imsearch.hpp>
#include <vis/utils/filesystem.hpp>

#include <QDebug>
#include <QProgressDialog>

vis::ImageSearch&
DemoGui::get() {
    auto range = service.equal_range(category);
    auto it = range.begin();
    for (auto end = range.end(); it != end; ++it) {
        QString t = str(vis::typeString(it->second->getType()));
        if (t == queryType)
            break;
    }
    Q_ASSERT(it != range.end());

    return *(it->second);
}

void
DemoGui::search() {
    qDebug() << "searching" << category << "by" << queryType;

    vis::ImageSearch& imsearch = get();

    arma::fmat query;
    if (realtimeCheckBox->isChecked()) {
        qDebug() << "computing descriptors ...";

        cv::Mat image = cv::imread(queryImagePath.string());
        query = imsearch.extract(image);

        qDebug() << "descriptors computed";
    }
    else {
        if (not loadQueries()) {
            messageBox("No queries file not found, please recompute queries.", QMessageBox::Critical);
            return;
        }
        Q_ASSERT(queryType == decodeType(queries->getType()));

        int queryId = queryList->currentRow();
        qDebug() << "query:" << queryId << queryNames[queryId];
        try {
            query = queries->data().col(queryId);
        } catch (...) {
            messageBox("Query file not found, please recompute queries.", QMessageBox::Critical);
            return;
        }
    }
    Q_ASSERT(query.n_cols == 1);

    std::vector<vis::Index::id_type> matches;
    imsearch.query(query, matches, results.size());

    PathList images = imsearch.get(matches, true);
    for (int n = 0; n < results.size(); n++) {
        setImage(results[n], images[n]);
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
    progress->setValue(1);

    vis::ImageSearch& imsearch = get();
    imsearch.build();
    progress->setValue(2);

    imsearch.save();
    progress->setValue(3);

    qDebug() << "indexing done!";
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

    vis::ImageSearch& imsearch = get();
    vis::Vocabulary* vocabulary = imsearch.getVocabulary();
    progress->setValue(1);

    queries.reset(new vis::Descriptors);
    extractDescriptors(category, queryType, names,
            queries.data(), vocabulary, [&](int i) {
                progress->setValue(i+2);
                qDebug() << "processing file" << str(names[i]);
            });

    fs::path savefile = queryFile(DATA_PATH, category, queryType);
    queries->save(savefile);
    progress->setValue(names.size());

    qDebug() << "queries done!";
}

