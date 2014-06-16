/**
 * @file demogui_images.cpp
 * @brief Qt Gui for the image search demo, show images dialog implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "ui_images.h"
#include "utils.hpp"

#include <vis/imsearch.hpp>
#include <QDebug>

void
DemoGui::showAll() {
    QDialog* dialog = new QDialog(0,0);

    Ui_Dialog imagesUi;
    imagesUi.setupUi(dialog);

    vis::ImageSearch& imsearch = get();
    PathList files = imsearch.getAll(true);
    foreach (const fs::path& file, files) {
        imagesUi.imagesList->addItem(new QListWidgetItem(QIcon(str(file)), ""));
    }

    dialog->show();
}

