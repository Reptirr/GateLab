#pragma once
#include <ComponentItem.h>

class SourceItem : public ComponentItem {
    qreal width_ = 75;
    qreal height_ = 75;

public:
    QRectF boundingRect() const override {
        return QRectF{0, 0, };
    };

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};