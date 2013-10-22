/**
 * @file demo.hpp
 * @brief Qt Gui for the image search demo
 * @author Paolo D'Apice
 */

#ifndef DEMO_HPP
#define DEMO_HPP

#include "ui_demo.h"
#include <QMainWindow>

class DemoGui : public QMainWindow, private Ui::MainWindow {
    Q_OBJECT

public:
    DemoGui();
    ~DemoGui();

};

#endif /* DEMO_HPP */

