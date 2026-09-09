#pragma once

#include <Mode.h>
#include <QToolBar>
#include <QActionGroup>

class GLToolBar : public QToolBar {
    Q_OBJECT
public:
    explicit GLToolBar(QWidget *parent = nullptr) : QToolBar("Modes", parent) {
        auto *actionGroup = new QActionGroup(this);
        actionGroup->setExclusive(true);

        setToolButtonStyle(Qt::ToolButtonTextUnderIcon); // text under icon in each action

        auto addModeAction = [&] (const QString& title, EditMode mode, const bool checked = false) {
            auto *action = new QAction{title, this};
            action->setCheckable(true);
            action->setChecked(checked);
            action->setData(QVariant::fromValue(mode));
            addAction(action);
            actionGroup->addAction(action);

            return action;
        };

        auto component_edit = addModeAction("Component editing", EditMode::COMPONENT_EDIT, true);
        auto wire_create = addModeAction("Wire create", EditMode::WIRE_CREATING);

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
                    const EditMode edit_mode = action->data().value<EditMode>();
                    emit modeChange(edit_mode);
                });
    }

signals:
    void modeChange(EditMode mode);
};
