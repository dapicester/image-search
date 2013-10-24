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

void
DemoGui::loadIndex() {
    fs::path loadfile = indexFile(DATA_PATH, category, queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "index not found: " << str(loadfile);
        messageBox("No index file found, please recompute index.", QMessageBox::Warning);
        descriptors.reset();
        return;
    }
    index.reset(vis::Index::load(loadfile));
    qDebug() << "index loaded from: " << str(loadfile);
}

void
DemoGui::loadDescriptors() {
    fs::path loadfile = descriptorsFile(DATA_PATH, category, queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "descriptors not found: " << str(loadfile);
        messageBox("No descriptors file found, please recompute descriptors.", QMessageBox::Warning);
        descriptors.reset();
        return;
    }
    descriptors.reset(vis::Descriptors::load(loadfile));
    qDebug() << "descriptors loaded from: " << str(loadfile);
}

void
DemoGui::loadVocabulary() {
    fs::path loadfile = vocabularyFile(DATA_PATH, category);
    if (not fs::exists(loadfile)) {
        qDebug() << "vocabulary not found: " << str(loadfile);
        messageBox("No vocabulary file found, please recompute vocabulary.", QMessageBox::Warning);
        vocabulary.reset();
        return;
    }
    vocabulary.reset(vis::Vocabulary::load(loadfile));
    qDebug() << "vocabulary loaded from: " << str(loadfile);
}


