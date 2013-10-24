/**
 * @file demogui_private.cpp
 * @brief Qt Gui for the image search demo, private functions implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"
#include <boost/filesystem/operations.hpp>
#include <boost/foreach.hpp>
#include <QMessageBox>
#include <QDebug>

void
DemoGui::loadQueryNames() {
    fs::path file = DATA_PATH / "test.txt";
    std::vector<std::string> queryNames = loadNames(file);
    BOOST_FOREACH(const std::string& name, queryNames) {
        names->append( QString(name.c_str()) );
    }
}

template <typename Container>
bool checkCategory(const Container& c, const QString& category) {
    return str(c.getCategory()) == category;
}

template <typename Container>
bool checkType(const Container& c, const QString& type) {
    return decodeType(c.getType()) == type;
}

void
DemoGui::loadImageNames() {
    QList<QRadioButton*> labels = categoryBox->findChildren<QRadioButton*>();
    for (QList<QRadioButton*>::iterator it = labels.begin(); it != labels.end(); ++it) {
        QString category = (*it)->text();

        fs::path file = categoryFile(DATA_PATH, category);
        fs::path dir = categoryDir(DATA_PATH, category);
        imagesMap[category] = loadNames(file, dir);
        qDebug() << "loaded" << imagesMap[category].size() << "images for" << category;
    }
}

bool
DemoGui::loadIndex() {
    if (index and checkCategory(*index, category) and checkType(*index, queryType)) {
        qDebug() << "index already loaded";
        return true;
    }

    fs::path loadfile = indexFile(DATA_PATH, category, queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "index not found:" << str(loadfile);
        messageBox("No index file found, please recompute index.", QMessageBox::Warning);
        descriptors.reset();
        return false;
    }
    index.reset(vis::Index::load(loadfile));

    qDebug() << "index loaded from:" << str(loadfile);
    Q_ASSERT_X(str(index->getCategory()) == category, "loadIndex", "index category mismatch");
    Q_ASSERT_X(decodeType(index->getType()) == queryType, "loadIndex", "index type mismatch");

    return true;
}

bool
DemoGui::loadDescriptors() {
    if (descriptors and checkCategory(*descriptors, category) and checkType(*descriptors, queryType)) {
        qDebug() << "descriptors already loaded";
        return true;
    }

    fs::path loadfile = descriptorsFile(DATA_PATH, category, queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "descriptors not found:" << str(loadfile);
        messageBox("No descriptors file found, please recompute descriptors.", QMessageBox::Warning);
        descriptors.reset();
        return false;
    }
    descriptors.reset(vis::Descriptors::load(loadfile));

    qDebug() << "descriptors loaded from:" << str(loadfile);
    Q_ASSERT_X(str(descriptors->getCategory()) == category, "loadDescriptors", "descriptors category mismatch");
    Q_ASSERT_X(decodeType(descriptors->getType()) == queryType, "loadDescriptors", "descriptors type mismatch");

    return true;
}

bool
DemoGui::loadQueries() {
    if (queries and checkType(*queries, queryType)) {
        qDebug() << "queries already loaded";
        return true;
    }

    fs::path loadfile = descriptorsFile(DATA_PATH, "test", queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "queries not found:" << str(loadfile);
        messageBox("No queries file found, please recompute queries.", QMessageBox::Warning);
        queries.reset();
        return false;
    }
    queries.reset(vis::Descriptors::load(loadfile));

    qDebug() << "queries loaded from:" << str(loadfile);
    Q_ASSERT_X(decodeType(queries->getType()) == queryType, "loadQueries", "queries type mismatch");

    return true;
}

bool
DemoGui::loadVocabulary() {
    if (vocabulary and checkCategory(*vocabulary, category)) {
        qDebug() << "vocabulary already loaded";
        return true;
    }

    fs::path loadfile = vocabularyFile(DATA_PATH, category);
    if (not fs::exists(loadfile)) {
        qDebug() << "vocabulary not found:" << str(loadfile);
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Warning);
        vocabulary.reset();
        return false;
    }
    vocabulary.reset(vis::Vocabulary::load(loadfile));
    qDebug() << "vocabulary loaded from:" << str(loadfile);
    return true;
}


