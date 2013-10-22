/**
 * @file main.cpp
 * @brief Image search demo.
 * @author Paolo D'Apice
 */

#include "demogui.hpp"
#include <QtGui>

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    DemoGui gui;
    gui.show();
    return app.exec();
}
