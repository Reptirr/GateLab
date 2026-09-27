#pragma once
#include <ComponentItem.h>

class SourceItem : public ComponentItem {

public:
    explicit SourceItem(const QPointF pos) : ComponentItem(75, 75) {
        setPos(pos);

        auto *pin = new PinItem{this};

        pin->setPos({
            rect().width() - pin->rect().width() / 2,
            rect().height() / 2 - pin->rect().height() / 2
        });

        pins_.push_back(pin);
    }

    QSizeF size() override {
        return QSizeF{width_, height_};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setPen(pen_);

        painter->drawRect(rect());
        painter->drawText(rect(), Qt::AlignCenter, "Source");
    }

    int type() const override {
        return SourceType;
    }
};