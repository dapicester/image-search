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

#include <QMainWindow>
#include <QScopedPointer>

#include <boost/ptr_container/ptr_map.hpp>

class QAbstractButton;
template <typename T> class QList;
class QListWidgetItem;
class QStringList;

namespace vis {
class ImageSearch;
}

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
    /// Show all images in category.
    void showAll();

    /// Search images.
    void search();

    /// Recompute index for selected category and query type.
    void recomputeIndex();

    /// Recompute query descriptors for selected category and query type.
    void recomputeQueries();

    /// Properly set image query names according to selected category.
    void setQueryNames(QAbstractButton*);

    /// Display the currently selected query image.
    void showQueryImage(int);

    /// Set the query type according to the selected option.
    void setQueryType(QAbstractButton*);

private:

    void initService();

    vis::ImageSearch& get();

    /// Display image into label.
    void setImage(QLabel* label, const fs::path& file);

    /// Load query image names.
    void loadQueryNames();

    /// Load query descriptors.
    bool loadQueries();

private:
    QStringList queryNames;                         ///< Names of the query images.

    QString category;                               ///< Currently selected category.
    QString queryType;                              ///< Currently selected query type.
    fs::path queryImagePath;

    QScopedPointer<vis::Descriptors> queries;       ///< Query descriptors for the current category and query type.

    typedef boost::ptr_multimap<QString, vis::ImageSearch> ServiceMap;
    ServiceMap service;

    QList<QLabel*> results;                         ///< Pointers to result labels displaying matching images.
};

#endif /* DEMO_HPP */

