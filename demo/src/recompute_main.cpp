/**
 * @file recompute_main.cpp
 * @brief Utility for recomputing descriptors and indexes (executable).
 * @author Paolo D'Apice
 */

#include "directories.hpp"
#include "recompute.hpp"

#include <QString>
#include <QtDebug>

int main(int argc, char** argv) {
    // available categories/types
    QVector<QString> CATEGORIES, TYPES;
    CATEGORIES << "bag" << "shoe" << "woman_shoe";
    TYPES << "color" << "shape" << "combined";

    QVector<QString> categories, types;

    switch (argc) {
    case 3:
        types << QString(argv[2]);
    case 2:
        categories << QString(argv[1]);
    case 1:
        break;
    default:
        qCritical("wrong number of arguments");
        return 1;
    }

    if (categories.empty()) categories = CATEGORIES;
    if (types.empty()) types = TYPES;

    qDebug() << "categories:" << categories;
    qDebug() << "types     :" << types;

    Recompute worker(DATA_DIR);
    foreach (const QString& category, categories) {
        if (not CATEGORIES.contains(category)) {
            qCritical() << "Skipping not supported category:" << category;
            continue;
        }
        foreach (const QString& type, types) {
            if (not TYPES.contains(type)) {
                qCritical() << "Skipping not supported type:" << type;
                continue;
            }
            worker.recompute(category, type);
        }
    }
}

