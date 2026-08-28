#pragma once

#include <QGraphicsItem>

#include "PinItem.h"

class PinItem;
class MainView;



class ComponentItem : public QGraphicsItem {
protected:
    // shows after drill-down to that scene; makes item drillable
    QGraphicsScene *interior_ = nullptr;

    std::vector<PinItem*> pins_;

    void paintPins(QPainter* painter) {
        for (PinItem* pin : pins()) {
            pin->paint(painter, {}, nullptr);
        }
    }

public:
    const QGraphicsScene* interior() const {
        return interior_;
    }

    std::vector<PinItem*> pins() const {
        return pins_;
    }
};
