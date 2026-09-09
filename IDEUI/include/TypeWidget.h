#pragma once

#include <QToolBar>

class TypeWidget : public QToolBar {
public:
    explicit TypeWidget(QWidget *parent = nullptr) : QToolBar(parent) {
        auto *component_mode = new QAction(this);
        addAction(component_mode);

        auto *wire_mode = new
    }
};
