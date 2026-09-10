#pragma once
#include <PinItem.h>
#include <QGraphicsItem>
#include <QPainter>
#include <QLineF>
#include <QPainterPath>
#include <set>
#include <WireItemHelpers.h>


class WireNode;
class WireLine;
class LogicWire;


class WireItem : public QGraphicsItem {
protected:
    std::unordered_set<WireLine*, WireLineHash, WireLineEqual> lines_;

private:
    QColor color_ = QColorConstants::Black;

    void setColorBySignal(bool signal);

    friend LogicWire;

public:
    WireItem(QGraphicsItem *from, QGraphicsItem *to);

    void addNode(WireLine *on_line, QPointF pos);
    void removeNode(const WireNode *node);
    void moveNode(WireNode *node, QPointF to_pos);

    QPainterPath shape() const override;
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type() const override;
};