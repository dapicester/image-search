/**
 * @file utils.cpp
 * @brief Utility functions for the image search demo.
 * @author Paolo D'Apice
 */

#include "utils.hpp"

#include <vis/callbacks.hpp>
#include <vis/descriptors.hpp>

#include <fstream>

#include <QMessageBox>
#include <QProgressDialog>

namespace fs = boost::filesystem;

using std::string;
using std::vector;

static const QString DATA_EXT = ".dgz";
static const QString TEXT_EXT = ".txt";

vector<fs::path>
queryNames(const vector<fs::path>& all, const QString& category) {
    vector<fs::path> names;
    QRegExp re("\\b" + category + "\\d");
    std::for_each(all.begin(), all.end(), [&](const fs::path& path) {
        if (str(path).contains(re)) names.push_back(path);
    });
    return names;
}

fs::path
queryFile(const fs::path& dataDir, const QString& category, const QString& type) {
    QString file = "query_";
    file.append(category).append("_").append(type);
    file.append(DATA_EXT);
    return dataDir / file.toStdString();
}

void
extractDescriptors(const QString& category, const QString& queryType,
        const std::vector<boost::filesystem::path>& names,
        vis::Descriptors* descriptors, vis::Vocabulary* vocabulary,
        vis::ProgressHandler handler) {
    // XXX quick'n dirty (TM)
    if (queryType == "color") {
        vis::HsvHistogramsCallback cb;
        descriptors->compute(category.toStdString(), names, cb,
                vis::ColorMode::COLORS, handler);
    }
    else if (queryType == "shape") {
        vis::HogBagOfWordsCallback cb(*vocabulary);
        descriptors->compute(category.toStdString(), names, cb,
                vis::ColorMode::GRAYSCALE, handler);
    }
    else if (queryType == "combined") {
        vis::CompositeCallback cb(*vocabulary);
        descriptors->compute(category.toStdString(), names, cb,
                vis::ColorMode::COLORS, handler);
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
progressDialog(const QString& title, QWidget* parent, int max) {
    QProgressDialog* dialog = new QProgressDialog(title, "Cancel", 0, max, parent);
    dialog->setWindowModality(Qt::WindowModal);
    dialog->setMinimumDuration(0);
    dialog->setCancelButton(0);
    dialog->show();
    return dialog;
}
