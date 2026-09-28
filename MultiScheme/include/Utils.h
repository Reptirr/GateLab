#pragma once
#include <PinItem.h>
#include <qguiapplication.h>
#include <qpoint.h>
#include <QSizeF>
#include <qstylehints.h>
#include <WireEndpoint.h>
#include <qpalette.h>
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


inline bool isDarkTheme() {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    const auto scheme = qGuiApp->styleHints()->colorScheme();
    const bool is_dark = (scheme == Qt::ColorScheme::Dark);
#else
    // check by making sure the window color is darkener than text
    const auto &pal = qGuiApp->palette();
    const bool is_dark = pal.color(QPalette::Window).lightness()
                        < pal.color(QPalette::WindowText).lightness();
#endif

    return is_dark;
}