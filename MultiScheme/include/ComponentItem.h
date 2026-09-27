#pragma once

#include <MoveHelper.h>
#include <qcoreapplication.h>
#include <QGraphicsSceneMouseEvent>
#include <qguiapplication.h>
#include <QtGui/qstylehints.h>

#include "PinItem.h"

class PinItem;
class MainView;



class ComponentItem : public QGraphicsItem {
    MoveHelper move_helper_;

protected:
    qreal width_{};
    qreal height_{};

    QPen pen_{};

    ComponentItem(const qreal width, const qreal height) : width_(width), height_(height) {
        pen_.setWidth(1);

        const auto scheme = qGuiApp->styleHints()->colorScheme();

        pen_.setColor(
            scheme == Qt::ColorScheme::Dark ? QColorConstants::White
                                              : QColorConstants::Black
        );
    }

    std::vector<PinItem*> pins_;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
        move_helper_.onMousePress(event->scenePos());
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override {
        setPos(move_helper_.onMouseMove(pos(), event->scenePos()));

        for (const auto *pin : pins_) pin->rebuildLine();
    }


public:
    virtual QGraphicsScene* interior() {
        return nullptr;
    }

    virtual QSizeF size() = 0;

    QRectF rect() const {
        return QRectF{0, 0, width_, height_};
    }

    QRectF boundingRect() const override {
        const qreal margin = pen_.widthF() + 10.0; // с запасом
        return rect().adjusted(-margin, -margin, margin, margin);
    }

    std::vector<PinItem*> pins() {
        return pins_;
    }

    ~ComponentItem() override {
        qDebug() << "component_item delete";
    }
};
