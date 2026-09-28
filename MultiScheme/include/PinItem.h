#pragma once
#include <my_assert.h>
#include <qpainter.h>
#include <UIConstants.h>
#include <WireItem.h>
#include <QGraphicsScene>
#include <WireEndpoint.h>
#include <WireLine.h>

class PinItem : public WireEndPoint {

    qreal width_ = 20;
    qreal height_ = 20;

    WireLine *conn_{};

    QPen pen_;

public:
    explicit PinItem(QGraphicsItem* parent) : WireEndPoint(parent) {
        setParentItem(parent);

        pen_ = (QColorConstants::Black);
        pen_.setWidth(3);
    }


    void addLine(WireLine *line) override {
        if (wire_item_)
            my_assert(wire_item_ == line->wire());

        wire_item_ = line->wire();
        conn_ = line;
    }
    void removeLine(WireLine *line) override {
        conn_ = nullptr;
        wire_item_ = nullptr; // there is no conn anymore
    }

    std::unordered_set<WireLine *> lines() override {
        // imitation
        if (conn_) return {conn_};
        else return {};
    }
    WireLine *conn() const {
        return conn_;
    }

    void clearLines() override {
        if (conn_) {
            conn_->removeWireEndpoint();
            conn_ = nullptr;
        }
    }
    void disconnect() override {
        conn_ = nullptr;
    }

    void rebuildLine() const {
        if (conn_) conn_->rebuild();
    }

    QRectF rect() const {
        return QRectF{0, 0, width_, height_};
    }

    QRectF boundingRect() const override {
        const qreal margin = pen_.widthF() / 2.0 + 1.0;
        return QRectF{0, 0, width_, height_}.adjusted(-margin, -margin, margin, margin);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setPen(pen_);

        painter->drawRect(0, 0, width_, height_);
    }

    int type() const override {
        return PinType;
    }

    ~PinItem() override {
        qDebug() << "pin_item delete";

        if (conn_) conn_->removeWireEndpoint();
    }
};
