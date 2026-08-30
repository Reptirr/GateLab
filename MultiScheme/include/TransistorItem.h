#pragma once
#include <ComponentItem.h>

class TransistorItem : public ComponentItem {

    qreal width_ = 150;
    qreal height_ = 100;

public:
    TransistorItem(QPointF pos) {
        setPos(pos);

        auto *top = new PinItem();
        auto *left = new PinItem();
        auto *right = new PinItem();

        auto x_center = (width_ - top->boundingRect().width()) / 2;
        auto y_center = (height_ - top->boundingRect().height()) / 2;

        left->setRelativePos(
            -(left->boundingRect().width() / 2),
            y_center,
            this
        );

        top->setRelativePos(
            x_center,
            -(top->boundingRect().height() / 2),
            this
        );

        right->setRelativePos(
            width_ - (right->boundingRect().width() / 2),
            y_center,
            this
        );

        pins_.push_back(left);
        pins_.push_back(top);
        pins_.push_back(right);
    }



    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    };

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->drawRect(0, 0, width_, height_);

        QRectF rect(0, 0, width_, height_);
        painter->drawText(rect, Qt::AlignCenter, "Transistor");
    };
};