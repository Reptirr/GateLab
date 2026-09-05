#pragma once
#include <qpainter.h>
#include <UIConstants.h>
#include <WireItem.h>

class PinItem : public QGraphicsItem {

    qreal width_ = 20;
    qreal height_ = 20;

    WireItem *wire_{};
    ComponentItem *owner_{};

public:
    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    }

    PinItem(ComponentItem *component_item) {
        owner_ = component_item;
    }

    void setWire(WireItem *wire) {
        wire_ = wire;
    }
    void removeWire() {
        wire_ = nullptr;
    }
    // returns nullptr if there is no wire
    WireItem *wire() const {
        return wire_;
    }
    ComponentItem *owner() const {
        return owner_;
    }

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
        pen.setWidth(3);
        painter->setPen(pen);

        painter->drawRect(0, 0, width_, height_);
    }

    int type() const override {
        return PinType;
    }
};
