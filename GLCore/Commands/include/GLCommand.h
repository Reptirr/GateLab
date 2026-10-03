#pragma once
#include <Mode.h>
#include <QUndoCommand>

class Controller;

class GLCommand : public QUndoCommand {
protected:
    Controller *controller_{};
    Mode *raw_mode_{};

public:
    explicit GLCommand(QUndoCommand *parent=nullptr) : QUndoCommand(parent) {}

    void init(Controller *controller, Mode *mode) {
        controller_ = controller;
        raw_mode_ = mode;
    }

    virtual ModeType needMode() = 0;
};
