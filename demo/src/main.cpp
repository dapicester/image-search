/**
 * @file main.cpp
 * @brief Image search demo.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"

#include <vis/configuration.hpp>
#include <QtGui>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    QStringList args = QCoreApplication::arguments();
    if (args.count() != 2) {
        std::cerr << "config file required" << std::endl;
        return 1;
    }

    std::string file = args.at(1).toStdString();
    vis::config::Configuration conf = vis::config::loadConfiguration(file);

    DemoGui gui(conf);
    gui.show();
    return app.exec();
}
