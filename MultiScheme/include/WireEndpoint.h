#pragma once

#include <QGraphicsItem>
#include <WireItem.h>

class WireLine;

class WireEndPoint : public QGraphicsItem {
protected:
    WireItem *wire_item_;
public:
    explicit WireEndPoint(QGraphicsItem *parent = nullptr) : QGraphicsItem(parent), wire_item_(dynamic_cast<WireItem *>(parent)) {

    }

    virtual void addLine(WireLine *line) = 0;
    virtual void removeLine(WireLine *line) = 0;

    virtual void clearLines() = 0;
    virtual void disconnect() = 0;

    void setParentWire(WireItem *wire) {
        wire_item_ = wire;
    }

    WireItem *parentWire() const {
        return wire_item_;
    }

    virtual std::unordered_set<WireLine *> lines() = 0;
};
