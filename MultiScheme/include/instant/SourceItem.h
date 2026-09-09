#pragma once
#include <ComponentItem.h>

class SourceItem : public ComponentItem {
    qreal width_ = 75;
    qreal height_ = 75;

public:
    explicit SourceItem(const QPointF pos) {
        setPos(pos);

        auto *pin = new PinItem{this};

        pin->setPos({
            SourceItem::boundingRect().width() - pin->boundingRect().width() / 2,
            SourceItem::boundingRect().height() / 2 - pin->boundingRect().height() / 2
        });

        pins_.push_back(pin);
    }

    QSizeF size() override {
        return QSizeF{width_, height_};
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