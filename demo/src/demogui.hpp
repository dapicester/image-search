/**
 * @file demo.hpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#ifndef DEMO_HPP
#define DEMO_HPP

#include "directories.hpp"
#include "ui_demo.h"
#include <vis.hpp>
#include <boost/filesystem/path.hpp>
#include <QMainWindow>
#include <QScopedPointer>

class QAbstractButton;
class QListWidgetItem;
class QStringList;

namespace fs = boost::filesystem;

namespace {
    static const fs::path DATA_PATH = fs::path(DATA_DIR);
}

class DemoGui : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    DemoGui();
    ~DemoGui();

public slots:
    void showAll();
    void search();

    /// Recompute index for selected category and query type.
    void recomputeIndex();

    /// Recompute descriptors for selected category and query type.
    void recomputeDescriptors();

    /// Recompute vocabulary for selected category.
    void recomputeVocabulary();

    /// Properly set image query names according to selected category.
    void setQueryNames(QAbstractButton*);

    /// Display the currently selected query image.
    void showQueryImage(int);

    /// Set the query type according to the selected option.
    void setQueryType(QAbstractButton*);

private:

    /// Load image queries from file.
    void loadQueryNames();

    /// Load index.
    void loadIndex();

    /// Load descriptors.
    void loadDescriptors();

    /// Load visual word vocabulary.
    void loadVocabulary();

private:
    QScopedPointer<QStringList> names;
    QString category;
    QString queryType;

    QScopedPointer<vis::Index> index;
    QScopedPointer<vis::Descriptors> descriptors;
    QScopedPointer<vis::Vocabulary> vocabulary;
};

#endif /* DEMO_HPP */

