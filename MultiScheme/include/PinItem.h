#pragma once
#include <QGraphicsItem>
#include <qpainter.h>

class PinItem : public QGraphicsItem {

    qreal width_ = 10;
    qreal height_ = 10;

public:
    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    };

    PinItem() = default;
    PinItem(const qreal x, const qreal y) {
        setPos(x, y);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        // make a border
        QPen pen(QColorConstants::Black);
        pen.setWidth(1);
        painter->setPen(pen);

        painter->drawRect(0, 0, width_, height_);
    };
};
