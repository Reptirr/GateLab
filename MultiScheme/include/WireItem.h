#pragma once
#include <PinItem.h>
#include <QGraphicsItem>
#include <qpainter.h>

class PinItem;


class WireItem : public QGraphicsItem {
protected:
    // must be on same scenes
    std::vector<PinItem*> pins_;

    QPointF lines_center_{};

public:
    WireItem() = default;

    QRectF boundingRect() const override {
        return QRectF();
    }

    void addPin(PinItem* pin) {
        pins_.push_back(pin);

        // центр пина в его локальных координатах -> в координаты сцены
        QPointF pinCenterScene = pin->mapToScene(pin->boundingRect().center());

        // из сцены -> в координаты this (т.к. линия - дочерний item this)
        QPointF pinCenterLocal = mapFromScene(pinCenterScene);

        if (pins_.size() == 2) {
            QPointF firstPinCenterScene = pins_[0]->mapToScene(pins_[0]->boundingRect().center());
            QPointF firstPinCenterLocal = mapFromScene(firstPinCenterScene);

            auto *first_line = new QGraphicsLineItem(QLineF(firstPinCenterLocal, pinCenterLocal), this);
            lines_center_ = first_line->line().center();
        } else if (pins_.size() > 2) {
            auto *new_line = new QGraphicsLineItem(QLineF(lines_center_, pinCenterLocal), this);
        }
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {}

    int type() const override {
        return WireType;
    }
};
