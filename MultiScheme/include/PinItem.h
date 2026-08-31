#pragma once
#include <QGraphicsItem>
#include <qpainter.h>
#include <UIConstants.h>

class PinItem : public QGraphicsItem {

    qreal width_ = 25;
    qreal height_ = 25;

public:
    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    }



    PinItem(const qreal x, const qreal y, QGraphicsItem* owner) {
        qreal owner_x = owner->pos().x();
        qreal owner_y = owner->pos().y();

        setPos(
            x + owner_x,
            y + owner_y
        );
    }
    PinItem() = default;

    void setRelativePos(qreal x, qreal y, QGraphicsItem* owner) {
        qreal owner_x = owner->pos().x();
        qreal owner_y = owner->pos().y();

        setPos(
            x + owner_x,
            y + owner_y
        );
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        // make a border
        QPen pen(QColorConstants::Black);
        pen.setWidth(1);
        painter->setPen(pen);

        painter->drawRect(0, 0, width_, height_);
    }

    int type() const override {
        return PinType;
    }
};
