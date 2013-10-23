/**
 * @file utils.cpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#include "utils.hpp"
#include <boost/foreach.hpp>
#include <QMessageBox>
#include <QProgressDialog>
#include <fstream>

namespace fs = boost::filesystem;

using std::string;
using std::vector;

static const QString DATA_EXT = ".dgz";
static const QString TEXT_EXT = ".txt";

vector<string>
loadNames(const fs::path& file) {
    vector<string> names;
    std::ifstream input(file.string().c_str());
    std::copy(std::istream_iterator<string>(input),
              std::istream_iterator<string>(),
              std::back_inserter(names));
    return names;
}

vector<fs::path>
loadNames(const fs::path& file, const fs::path& prefix) {
    vector<string> lines = loadNames(file);
    vector<fs::path> names;
    BOOST_FOREACH(const string& line, lines) {
        names.push_back(prefix / line);
    }
    return names;
}

fs::path
categoryFile(const fs::path& dataDir, const QString& category) {
    QString file(category);
    file.append(TEXT_EXT);
    return dataDir / file.toStdString();
}

fs::path
categoryDir(const fs::path& dataDir, const QString& category) {
    return dataDir / category.toStdString();
}

fs::path
vocabularyFile(const fs::path& dataDir, const QString& category) {
    QString file = "vocabulary_";
    file.append(category);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

fs::path
descriptorsFile(const fs::path& dataDir, const QString& category, const QString& type) {
    QString file = "descriptors_";
    file.append(type).append("_").append(category);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

fs::path
indexFile(const fs::path& dataDir, const QString& category, const QString& type) {
    QString file = "index_";
    file.append(type).append("_").append(category);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

bool
confirmMessageBox(const QString& message) {
    QMessageBox msg;
    msg.setText(message);
    msg.setInformativeText("Are you sure?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);
    switch(msg.exec()) {
    case QMessageBox::Ok:
        return true;
    default:
        return false;
    }
}

QProgressDialog*
progressDialog(const QString& title, QWidget* parent, int min, int max) {
    QProgressDialog* dialog = new QProgressDialog(title, "Cancel", min, max, parent);
    //dialog->setWindowModality(Qt::WindowModal);
    dialog->show();
    return dialog;
}
