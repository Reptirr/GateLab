#pragma once
#include <qgraphicsitem.h>


enum {
    // basic
    PinType = QGraphicsItem::UserType + 1,
    WireType = QGraphicsItem::UserType + 2,

    // derived
    TransistorType = QGraphicsItem::UserType + 3,
    SourceType = QGraphicsItem::UserType + 4
};
