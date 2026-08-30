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

public:
    QGraphicsScene* interior() {
        return interior_;
    }

    const std::vector<PinItem*>& pins() {
        return pins_;
    }
};
