#pragma once
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

public:
    explicit PinItem(QGraphicsItem* parent) : WireEndPoint(parent) {
        setParentItem(parent);
    }


    void addLine(WireLine *line) override {
        if (wire_item_)
            assert(wire_item_ == line->wire());

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

    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        // make a border
        QPen pen(QColorConstants::Black);
        pen.setWidth(3);
        painter->setPen(pen);

        painter->drawRect(0, 0, width_, height_);
    }

    int type() const override {
        return PinType;
    }

    ~PinItem() override {
        if (wire_item_) wire_item_->removeEndPoint(this);
    }
};
