/**
 * @file demogui.cpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "directories.hpp"
#include "utils.hpp"
#include <vocabulary.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <QDebug>
#include <QProgressDialog>

namespace fs = boost::filesystem;

#define connectbtn(BTN, FUNC) connect(BTN, SIGNAL(clicked()), this, SLOT(FUNC))

namespace {
    static const fs::path DATA_PATH = fs::path(DATA_DIR);
}

DemoGui::DemoGui() :
        names(new QStringList) {
    setupUi(this);

    // connect signals-slots
    connectbtn(searchButton, search());
    connectbtn(showAllButton, showAll());
    connectbtn(recomputeIndexButton, recomputeIndex());
    connectbtn(recomputeDescriptorsButton, recomputeDescriptors());
    connectbtn(recomputeVocabularyButton, recomputeVocabulary());

    connect(categoryGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(setQueryNames(QAbstractButton*)));
    connect(queryList, SIGNAL(currentRowChanged(int)),
            this, SLOT(showQueryImage(int)));
    connect(queryTypeGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(setQueryType(QAbstractButton*)));

    // other gui initializations
    loadQueryNames();
    setQueryNames(categoryGroup->checkedButton());
    setQueryType(queryTypeGroup->checkedButton());

    // other initializations
    loadIndex();
    loadVocabulary();
}

DemoGui::~DemoGui() {
}

void
DemoGui::loadQueryNames() {
    fs::path file = DATA_PATH / "test.txt";
    std::vector<std::string> queryNames = loadNames(file);
    BOOST_FOREACH(const std::string& name, queryNames) {
        names->append( QString(name.c_str()) );
    }
}

void
DemoGui::setQueryNames(QAbstractButton* button) {
    category = button->text();
    qDebug() << "category: " << category;
    QStringList queryNames = names->filter(QRegExp("^" + category));

    queryList->clear();
    queryList->addItems(queryNames);
    queryList->setCurrentRow(0);
}

void
DemoGui::showQueryImage(int row) {
    if (row < 0) {
        // list content has changed
        return;
    }

    QString name = queryList->item(row)->text();
    //qDebug() << "query: " << name;
    fs::path file = DATA_PATH / "test" / name.toStdString();
    QPixmap image(file.string().c_str());
    queryImage->setPixmap(image.scaled(queryImage->size(), Qt::KeepAspectRatio));
}

void
DemoGui::setQueryType(QAbstractButton* button) {
    queryType = button->text();
    qDebug() << "queryType: " << queryType;
}

void
DemoGui::loadIndex() {
    fs::path loadfile = indexFile(DATA_PATH, category, queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "index not found!";
        return;
    }
    index.reset(vis::Index::load(loadfile));
}

void
DemoGui::loadVocabulary() {
    fs::path loadfile = vocabularyFile(DATA_PATH, category);
    if (not fs::exists(loadfile)) {
        qDebug() << "vocabulary not found!";
        return;
    }
    vocabulary.reset(vis::Vocabulary::load(loadfile));
}

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
    // TODO
    qDebug() << "recompute index";
}

void
DemoGui::recomputeDescriptors() {
    // TODO
    qDebug() << "recompute descriptors";
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
    qDebug() << "file: " << file.string().c_str();
    fs::path dir = categoryDir(DATA_PATH, category);
    qDebug() << "dir: " << dir.string().c_str();
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
