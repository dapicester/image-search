/**
 * @file utils.cpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#include "utils.hpp"

#include <vis/callbacks.hpp>
#include <vis/descriptors.hpp>

#include <boost/foreach.hpp>
#include <fstream>

#include <QMessageBox>
#include <QProgressDialog>

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

vector<fs::path>
queryNames(const vector<fs::path>& all, const QString& category) {
    vector<fs::path> names;
    QRegExp re("\\b" + category + "\\d");
    for (vector<fs::path>::const_iterator it = all.begin(); it != all.end(); ++it) {
        if (str(*it).contains(re)) {
            names.push_back(*it);
        }
    }
    return names;
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
    file.append(category).append("_").append(type);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

fs::path
queryFile(const fs::path& dataDir, const QString& category, const QString& type) {
    QString file = "query_";
    file.append(category).append("_").append(type);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

fs::path
indexFile(const fs::path& dataDir, const QString& category, const QString& type) {
    QString file = "index_";
    file.append(category).append("_").append(type);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

void
extractDescriptors(const QString& category, const QString& queryType,
        const std::vector<boost::filesystem::path>& names,
        vis::Descriptors* descriptors, vis::Vocabulary* vocabulary) {
    // XXX quick'n dirty (TM)
    if (queryType == "color") {
        vis::HsvHistogramsCallback cb;
        descriptors->compute(category.toStdString(), names, cb);
    }
    else if (queryType == "shape") {
        vis::HogBagOfWordsCallback cb(*vocabulary);
        descriptors->compute(category.toStdString(), names, cb);
    }
    else if (queryType == "combined") {
        vis::CompositeCallback cb(*vocabulary);
        descriptors->compute(category.toStdString(), names, cb);
    }
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

void
messageBox(const QString& message, QMessageBox::Icon icon) {
    QMessageBox msg;
    msg.setText(message);
    msg.setIcon(icon);
    msg.exec();
}

QProgressDialog*
progressDialog(const QString& title, QWidget* parent, int min, int max) {
    QProgressDialog* dialog = new QProgressDialog(title, "Cancel", min, max, parent);
    //dialog->setWindowModality(Qt::WindowModal);
    dialog->show();
    return dialog;
}
