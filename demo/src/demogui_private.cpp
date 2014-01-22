/**
 * @file demogui_private.cpp
 * @brief Qt Gui for the image search demo, private functions implementation.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include "utils.hpp"

#include <boost/filesystem/operations.hpp>
#include <vis/imsearch.hpp>
#include <vis/utils/filesystem.hpp>

#include <QDebug>

void
DemoGui::initService() {
    std::for_each(config.categories.begin(),
                  config.categories.end(),
                  [&](const vis::config::Category& category) {
        QString cat = str(category.name);
        for (const std::string& type : category.type) {
            qDebug() << "Adding" << cat << "/" << str(type);

            vis::DescriptorsType dt = vis::toDescriptorsType(type);
            vis::ImageSearch* imsearch = new vis::ImageSearch(category.name, dt, DATA_PATH);
            imsearch->load();

            service.insert(cat, imsearch);
        }
    });
}

void
DemoGui::loadQueryNames() {
    fs::path file = vis::categoryFile(DATA_PATH, "test");
    std::vector<std::string> names = vis::loadNames(file);
    foreach(const std::string& name, names) {
        queryNames << QString(name.c_str());
    }
}

template <typename Container>
bool checkCategory(const Container& c, const QString& category) {
    return str(c.getCategory()) == category;
}

template <typename Container>
bool checkType(const Container& c, const QString& type) {
    return decodeType(c.getType()) == type;
}

bool
DemoGui::loadQueries() {
    if (queries and checkCategory(*queries, category) and checkType(*queries, queryType)) {
        qDebug() << "queries already loaded";
        return true;
    }

    fs::path loadfile = queryFile(DATA_PATH, category, queryType);
    if (not fs::exists(loadfile)) {
        qDebug() << "queries not found:" << str(loadfile);
        queries.reset();
        return false;
    }
    queries.reset(vis::Descriptors::load(loadfile));

    qDebug() << "queries loaded from:" << str(loadfile);
    Q_ASSERT_X(decodeType(queries->getType()) == queryType, "loadQueries", "queries type mismatch");

    return true;
}

