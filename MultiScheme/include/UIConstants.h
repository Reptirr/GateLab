#pragma once
#include <qgraphicsitem.h>


enum {
    // basic
    PinType = QGraphicsItem::UserType + 10,
    WireType = QGraphicsItem::UserType + 20,

    // derived
    TransistorType = QGraphicsItem::UserType + 30,
    SourceType = QGraphicsItem::UserType + 40
};
