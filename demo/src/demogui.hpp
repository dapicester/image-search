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
template <typename T> class QList;
class QListWidgetItem;
class QStringList;

namespace fs = boost::filesystem;

namespace {
    static const fs::path DATA_PATH = fs::path(DATA_DIR);
}

typedef std::vector<fs::path> PathList;

/// @brief GUI for the demo application of the Visual Image Search.
class DemoGui : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    DemoGui();
    ~DemoGui();

public slots:
    void showAll();

    /// Search images.
    void search();

    /// Recompute index for selected category and query type.
    void recomputeIndex();

    /// Recompute descriptors for selected category and query type.
    void recomputeDescriptors();

    /// Recompute query descriptors for selected category and query type.
    void recomputeQueries();

    /// Recompute vocabulary for selected category.
    void recomputeVocabulary();

    /// Properly set image query names according to selected category.
    void setQueryNames(QAbstractButton*);

    /// Display the currently selected query image.
    void showQueryImage(int);

    /// Set the query type according to the selected option.
    void setQueryType(QAbstractButton*);

private:

    /// Display image into label.
    void setImage(QLabel* label, const fs::path& file);

    /// Load query image names.
    void loadQueryNames();

    /// Load image names.
    void loadImageNames();

    /// Load index.
    bool loadIndex();

    /// Load descriptors.
    bool loadDescriptors();

    /// Load query descriptors.
    bool loadQueries();

    /// Load visual word vocabulary.
    bool loadVocabulary();

private:
    QStringList queryNames;                         ///< Names of the query images.
    QMap<QString, PathList> imagesMap;              ///< Full path to indexed images by category.

    QString category;                               ///< Currently selected category.
    QString queryType;                              ///< Currently selected query type.

    QScopedPointer<vis::Index> index;               ///< Index for the current category and query type.
    QScopedPointer<vis::Descriptors> descriptors;   ///< Descriptors for the current category and query type.
    QScopedPointer<vis::Descriptors> queries;       ///< Query descriptors for the current category and query type.
    QScopedPointer<vis::Vocabulary> vocabulary;     ///< Vocabulary for the current category and query type.

    QList<QLabel*> results;                         ///< Pointers to result labels displaying matching images.
};

#endif /* DEMO_HPP */

