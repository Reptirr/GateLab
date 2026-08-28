#pragma once

#include <QGraphicsItem>

#include "PinItem.h"

class PinItem;
class MainView;

class SchemeItem : public QGraphicsItem {


protected:
    // shows after drill-down to that item
    QGraphicsScene *_interior = nullptr;

    // get pins (pos)
    virtual std::vector<PinItem*> pins() = 0;

    // paint pins
    void paintPins(QPainter* painter) {
        for (PinItem* pin : pins()) {
            pin->paint(painter, {}, nullptr);
        }
    }

    friend MainView;

};
