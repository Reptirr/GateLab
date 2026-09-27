#pragma once
#include <ComponentItem.h>

class NTransistorItem : public ComponentItem {

public:
    explicit NTransistorItem(const QPointF pos) : ComponentItem(150, 100) {
        setPos(pos);

        auto *top = new PinItem{this};
        auto *left = new PinItem{this};
        auto *right = new PinItem{this};

        left->setPos({
            -left->rect().width() / 2,
            rect().height() / 2 - left->rect().height() / 2
        });

        top->setPos({
            rect().width() / 2 - top->rect().width() / 2,
            -top->rect().height() / 2
        });

        right->setPos({
            rect().width() - right->rect().width() / 2,
            rect().height() / 2 - right->rect().height() / 2
        });

        pins_.push_back(left);
        pins_.push_back(top);
        pins_.push_back(right);
    }

    constexpr QSizeF size() override {
        return QSizeF{width_, height_};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setPen(pen_);

        painter->drawRect(rect());
        painter->drawText(rect(), Qt::AlignCenter, "NTransistor");
    }

    int type() const override {
        return NTransistorType;
    }
};