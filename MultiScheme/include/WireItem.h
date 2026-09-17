#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <QLineF>
#include <QPainterPath>
#include <set>
#include <unordered_map>


class WireLine;
class WireNode;
class WireEndPoint;
class PinItem;
class LogicWire;


// always need 2 end points
class WireItem : public QGraphicsItem {
    QColor color_ = QColorConstants::Black;

    void setColorBySignal(bool signal);
    friend LogicWire;

    // graph
    std::unordered_set<WireEndPoint *> end_points_;

    // deleting
    void notifyEndPointDelete() const;

public:
    WireItem(WireEndPoint *end_point1, WireEndPoint *end_point2);

    void createNode(const WireLine *on_line, QPointF pos); // create node on line
    void collapseNode(WireNode *node); // remove node and combine 2 lines to 1
    void removeEndPoint(WireEndPoint *end_point);

    void createLine(WireEndPoint *from, WireEndPoint *to); // create line from our node to other end_point

    QColor color() const;
    bool empty() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type() const override;

    ~WireItem() override;
};