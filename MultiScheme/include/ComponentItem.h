#pragma once

#include <QGraphicsItem>

#include "PinItem.h"

class PinItem;
class MainView;



class ComponentItem : public QGraphicsItem {
protected:
    std::vector<PinItem*> pins_;

public:
    virtual QGraphicsScene* interior() {
        return nullptr;
    }

    std::vector<PinItem*> pins() {
        return pins_;
    }
};
