#pragma once
#include <GLCommand.h>
#include <QObject>
#include <ranges>

class ModeManager : public QObject {
    Q_OBJECT

    QUndoStack stack_;

    Controller *controller_{};
    std::unordered_map<ModeType, Mode*> modes_{};

public slots:
    void onCommand(GLCommand *command) {
        command->init(controller_, modes_.at(command->needMode()));

        stack_.push(command);
    }

public:
    explicit ModeManager(Controller *controller) : controller_(controller) {
        // init modes_
        auto *component_mode = new ComponentMode();
        modes_.insert({ ModeType::COMPONENTS, component_mode });
    }

    QAction *createUndoAction(QWidget *parent) const {
        auto *act = stack_.createUndoAction(parent);
        act->setShortcut(QKeySequence::Undo);

        return act;
    }
    QAction *createRedoAction(QWidget *parent) const {
        auto *act = stack_.createRedoAction(parent);
        act->setShortcuts(QKeySequence::Redo);

        return act;
    }

    ~ModeManager() override {
        for (auto val : std::views::values(modes_)) {
            delete val;
        }
    }
};
