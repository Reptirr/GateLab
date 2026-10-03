#pragma once

#include <GLCore.h>
#include <GLToolBar.h>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    // GL
    GLCore *gl_core_{};

    // internals
    GLModeToolBar *toolbar_;

public:
    MainWindow() : toolbar_(new GLModeToolBar{this}) {
        gl_core_ = new GLCore();

        // toolbar
        addToolBar(toolbar_);

        // central
        setCentralWidget(gl_core_->view());
    }

    ~MainWindow() override {
        delete gl_core_;
    }
};