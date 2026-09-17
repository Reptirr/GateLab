#pragma once

#include <MoveHelper.h>
#include <QGraphicsSceneMouseEvent>

#include "PinItem.h"

class PinItem;
class MainView;



class ComponentItem : public QGraphicsItem {
    MoveHelper move_helper_;

protected:
    std::vector<PinItem*> pins_;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        move_helper_.onMousePress(event->scenePos());
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        setPos(move_helper_.onMouseMove(pos(), event->scenePos()));

        for (const auto *pin : pins_) pin->rebuildLine();
    }


public:
    virtual QGraphicsScene* interior() {
        return nullptr;
    }

    virtual QSizeF size() = 0;

    std::vector<PinItem*> pins() {
        return pins_;
    }

    ~ComponentItem() override {
        qDebug() << "component_item delete";

        // delete pins
        for (const auto *pin : pins_) {
            delete pin;
        }
    }
};
