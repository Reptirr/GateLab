#pragma once
#include <ComponentItem.h>

class TransistorItem : public ComponentItem {

    qreal width_ = 150;
    qreal height_ = 100;

public:
    TransistorItem(QPointF pos) {
        setPos(pos);

        auto *top = new PinItem{this};
        auto *left = new PinItem{this};
        auto *right = new PinItem{this};

        const auto x_center = (width_ - top->boundingRect().width()) / 2;
        const auto y_center = (height_ - top->boundingRect().height()) / 2;

        left->setPos(
            -(left->boundingRect().width() / 2),
            y_center
        );

        top->setPos(
            x_center,
            -(top->boundingRect().height() / 2)
        );

        right->setPos(
            width_ - (right->boundingRect().width() / 2),
            y_center
        );

        pins_.push_back(left);
        pins_.push_back(top);
        pins_.push_back(right);
    }

    constexpr QSizeF size() override {
        return QSizeF{width_, height_};
    }

    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->drawRect(0, 0, width_, height_);

        const QRectF rect(0, 0, width_, height_);
        painter->drawText(rect, Qt::AlignCenter, "Transistor");
    }

    int type() const override {
        return TransistorType;
    }
};