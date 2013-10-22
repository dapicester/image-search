/**
 * @file demo.hpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#ifndef DEMO_HPP
#define DEMO_HPP

#include "ui_demo.h"
#include <QMainWindow>
#include <QScopedPointer>

class QAbstractButton;
class QListWidgetItem;
class QStringList;

namespace vis {
    class Vocabulary;
}

class DemoGui : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    DemoGui();
    ~DemoGui();

public slots:
    void showAll();
    void search();

    void recomputeIndex();
    void recomputeDescriptors();
    void recomputeVocabulary();

    /// Properly set image query names according to selected category.
    void setQueryNames(QAbstractButton*);

    /// Display the currently selected query image.
    void showQueryImage(int);

    void setQueryType(QAbstractButton*);

private:
    /// Load image queries from file.
    void loadQueryNames();

    /// Load visual word vocabulary.
    void loadVocabulary();

private:
    QScopedPointer<QStringList> names;
    QString category;
    QString queryType;

    QScopedPointer<vis::Vocabulary> vocabulary;
};

#endif /* DEMO_HPP */

