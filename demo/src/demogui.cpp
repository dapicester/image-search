/**
 * @file demogui.cpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "directories.hpp"
#include "utils.hpp"
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <QDebug>

namespace fs = boost::filesystem;

#define connectbtn(BTN, FUNC) connect(BTN, SIGNAL(clicked()), this, SLOT(FUNC))

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

    // other initializations
    loadQueryNames();
    setQueryNames(categoryGroup->checkedButton());
}

DemoGui::~DemoGui() {
}

void
DemoGui::loadQueryNames() {
    fs::path file = fs::path(DATA_DIR) / "test.txt";
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
    fs::path file = fs::path(DATA_DIR) / "test" / name.toStdString();
    QPixmap image(file.string().c_str());
    queryImage->setPixmap(image.scaled(queryImage->size(), Qt::KeepAspectRatio));
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
    // TODO
    qDebug() << "recompute vocabulary";
}
