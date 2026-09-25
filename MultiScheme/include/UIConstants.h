#pragma once

#include <qgraphicsitem.h>

// sizes
constexpr QRectF pin_rect = {0, 0, 20, 20};
constexpr QRectF node_rect = {0, 0, 15, 15};


// types for qgraphics_cast
enum {
    // basic
    PinType = QGraphicsItem::UserType + 10,
    WireType = QGraphicsItem::UserType + 20,

    // derived
    TransistorType = QGraphicsItem::UserType + 30,
    NTransistorType = QGraphicsItem::UserType + 35,
    SourceType = QGraphicsItem::UserType + 40
};


// wire graph z values
enum {
    WireZValue = 5,
    LineZValue = 6,
    NodeZValue = 7,

    ComponentZValue = 10,
    PinZValue = 11
};
