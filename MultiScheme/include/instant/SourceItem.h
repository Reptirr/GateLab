#pragma once
#include <ComponentItem.h>

class SourceItem : public ComponentItem {
    qreal width_ = 75;
    qreal height_ = 75;

public:
    SourceItem(QPointF pos) {
        setPos(pos);

        auto *pin = new PinItem{};

        auto y_center = (height_ - pin->boundingRect().width()) / 2;

        pin->setRelativePos(
            width_ - pin->boundingRect().width()/2,
            y_center,
            this
        );

        pins_.push_back(pin);
    }

    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->drawRect(0, 0, width_, height_);

        QRectF rect(0, 0, width_, height_);
        painter->drawText(rect, Qt::AlignCenter, "Source");
    }

    int type() const override {
        return SourceType;
    }
};