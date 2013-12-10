/**
 * @file demogui.cpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"

#include <QDebug>

#define connectbtn(BTN, FUNC) connect(BTN, SIGNAL(clicked()), this, SLOT(FUNC))

DemoGui::DemoGui() {
    setupUi(this);
    this->setFixedSize(this->size());

    // XXX very quick'n dirty (TM)
    results.append(queryResult_1);
    results.append(queryResult_2);
    results.append(queryResult_3);
    results.append(queryResult_4);
    results.append(queryResult_5);
    results.append(queryResult_6);
    results.append(queryResult_7);
    results.append(queryResult_8);
    results.append(queryResult_9);
    results.append(queryResult_10);
    results.append(queryResult_11);
    results.append(queryResult_12);
    results.append(queryResult_13);
    results.append(queryResult_14);
    results.append(queryResult_15);

    for (int i = 0; i < results.size(); i++)
        results[i]->setText(QString::number(i));

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
    loadImageNames();
    setQueryNames(categoryGroup->checkedButton());
    setQueryType(queryTypeGroup->checkedButton());

    // other initializations
}

DemoGui::~DemoGui() {
}

void
DemoGui::setQueryNames(QAbstractButton* button) {
    category = button->text();
    QStringList names = queryNames.filter(QRegExp("^" + category));

    queryList->clear();
    queryList->addItems(names);
    queryList->setCurrentRow(0);
}

void
DemoGui::showQueryImage(int row) {
    if (row < 0) {
        // list content has changed
        return;
    }

    QString name = queryList->item(row)->text();
    static fs::path dir = DATA_PATH / "test";
    queryImagePath = dir / name.toStdString();
    setImage(queryImage, queryImagePath);
}

void
DemoGui::setQueryType(QAbstractButton* button) {
    queryType = button->text();
}

void
DemoGui::setImage(QLabel* label, const fs::path& file) {
    QPixmap image(file.string().c_str());
    label->setPixmap(image.scaled(label->size(), Qt::KeepAspectRatio));
}

