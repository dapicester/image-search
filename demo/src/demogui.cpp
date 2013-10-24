/**
 * @file demogui.cpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include <QDebug>

#define connectbtn(BTN, FUNC) connect(BTN, SIGNAL(clicked()), this, SLOT(FUNC))

DemoGui::DemoGui() :
        names(new QStringList) {
    setupUi(this);
    this->setFixedSize(this->size());

    // connect signals-slots
    connectbtn(searchButton, search());
    connectbtn(showAllButton, showAll());
    connectbtn(recomputeIndexButton, recomputeIndex());
    connectbtn(recomputeDescriptorsButton, recomputeDescriptors());
    connectbtn(recomputeQueriesButton, recomputeQueries());
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
}

DemoGui::~DemoGui() {
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

