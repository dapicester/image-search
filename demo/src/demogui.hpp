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
class QStringList;

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

private:
    /// Load image queries from file.
    void loadQueryNames();


private:
    QScopedPointer<QStringList> names;
    QString category;
};

#endif /* DEMO_HPP */

