#pragma once

#include <QGraphicsItem>

#include "Pin.h"

class PinItem;
class MainView;

class SchemeItem : public QGraphicsItem {

protected:
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
