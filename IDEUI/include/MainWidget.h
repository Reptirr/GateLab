#pragma once
#include <Controller.h>
#include <QWidget>
#include <QVBoxLayout>

class MainWidget : public QWidget {

    Controller controller_;

public:
    MainWidget() {
        auto *layout = new QVBoxLayout();

        // add ide ui
    }
};