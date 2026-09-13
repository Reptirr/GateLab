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

    virtual std::unordered_set<WireLine *> lines() = 0;
};
