#pragma once
#include <QGraphicsItem>
#include <WireEndpoint.h>

class WireLine;

class WireNode : public WireEndPoint {
    std::unordered_set<WireLine *> lines_;

    qreal width_ = 15;
    qreal height_ = 15;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
    explicit WireNode(WireItem *parent_wire);

    void addLine(WireLine *line) override;
    void removeLine(WireLine *line) override;

    std::unordered_set<WireLine *> lines() override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    ~WireNode() override;
};
