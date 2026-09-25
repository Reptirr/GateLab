#pragma once

#include <Controller.h>
#include <GLToolBar.h>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    // externals
    Controller controller_;

    // internals
    GLToolBar *toolbar_;

    void initConnects() {
        // GLToolBar -> InputMapper
        connect(toolbar_, &GLToolBar::modeChange,
                controller_.input_mapper(), &InputMapper::onModeChange);
    }

public:
    MainWindow() : toolbar_(new GLToolBar{this}) {
        // toolbar
        addToolBar(toolbar_);

        // central
        setCentralWidget(controller_.view());

        initConnects();
    }
};