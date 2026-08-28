#pragma once
#include <PinItem.h>
#include <QGraphicsItem>
#include <qpainter.h>

class PinItem;


class WireItem : public QGraphicsItem {
protected:
    // must be on same scenes
    std::vector<PinItem*> pins_;

public:
    WireItem() = default;

    explicit WireItem(const std::vector<PinItem*> &pins) {
        pins_ = pins;
    }

    QRectF boundingRect() const override {
        return QRectF();
    }

    void addPin(PinItem* pin) {
        pins_.push_back(pin);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        // it`s need 2 pins to draw
        if (pins_.size() < 2) return;

        // make line between pins 0, 1
        auto *first_line = new QGraphicsLineItem( QLineF(pins_[0]->pos(), pins_[1]->pos()), this );
        first_line->setParentItem(this);
        scene()->addItem(first_line);

        // make other lines from first line center
        for (int i = 2; i < pins_.size(); i++) {
            auto *line = new QGraphicsLineItem( QLineF(first_line->line().center(), pins_[i]->pos()), this );
            line->setParentItem(this);
            scene()->addItem(line);
        }

    }
};
