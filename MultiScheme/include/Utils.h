#pragma once
#include <PinItem.h>
#include <qpoint.h>
#include <QSizeF>
#include <WireEndpoint.h>
#include <WireNode.h>

inline QPointF centerPos(const QPointF top_left, const QSizeF size) {
    return {
        top_left.x() + size.width()/2,
        top_left.y() + size.height()/2
    };
}

inline PinItem * instanceOfPinItem(WireEndPoint *end_point) {
    return dynamic_cast<PinItem *>(end_point);
}

inline WireNode * instanceOfWireNode(WireEndPoint *end_point) {
    return dynamic_cast<WireNode *>(end_point);
}

inline QPointF closestPointOnLine(const QLineF &line, const QPointF &point) {
    const QPointF a = line.p1();
    const QPointF b = line.p2();

    const QPointF ab = b - a;
    const qreal abLenSq = QPointF::dotProduct(ab, ab);

    if (abLenSq == 0.0) {
        return a; // отрезок вырожден в точку
    }

    const QPointF ap = point - a;
    qreal t = QPointF::dotProduct(ap, ab) / abLenSq;

    t = std::clamp(t, 0.0, 1.0); // ограничиваем проекцию концами отрезка

    return a + t * ab;
}