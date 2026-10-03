#pragma once

#include <Mode.h>
#include <QToolBar>
#include <QActionGroup>

class GLModeToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit GLModeToolBar(QWidget *parent = nullptr) : QToolBar("Modes", parent) {
        auto *actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);

        setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // text under icon in each action

        auto addModeAction = [&] (const QString& title, ModeType mode, const bool checked = false) {
            auto *action = new QAction{title, this};
            action->setCheckable(true);
            action->setChecked(checked);
            action->setData(QVariant::fromValue(mode));
            addAction(action);
            actionGroup->addAction(action);

            return action;
        };

        auto component_edit = addModeAction("Component editing", ModeType::COMPONENTS, true);
        auto wire_create = addModeAction("Wire create", ModeType::WIRES);

        component_edit->setShortcuts({
            {Qt::Key_C},
            {Qt::Key_1}
        });
        wire_create->setShortcuts({
            {Qt::Key_W},
            {Qt::Key_2}
        });

        connect(actionGroup, &QActionGroup::triggered,
                this, [&](const QAction *action) {
                    const ModeType edit_mode = action->data().value<ModeType>();
                    emit modeChange(edit_mode);
                });
    }

signals:
    void modeChange(ModeType mode);
};
