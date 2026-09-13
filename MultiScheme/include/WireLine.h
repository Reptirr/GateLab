#pragma once
#include <QGraphicsItem>

class WireItem;
class WireEndPoint;

// item who paint rhombus and say wire create new node
class WireLine : public QGraphicsItem {
    WireEndPoint *from_;
    WireEndPoint *to_;

    WireItem *parent_wire_;

    QLineF line_;

    QGraphicsPolygonItem *romb_{};

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

public:
    WireLine(WireEndPoint *from, WireEndPoint *to, WireItem *parent_wire);

    WireEndPoint *from() const;
    WireEndPoint *to() const;

    WireItem *wire() const;

    void rebuild();

    QPainterPath shape() const override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // just alias for `delete this;`
    void removeWireEndpoint() const;
    ~WireLine() override;
};
