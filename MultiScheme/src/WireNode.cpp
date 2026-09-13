#include <qgraphicssceneevent.h>
#include <QPainter>
#include <WireLine.h>
#include <WireNode.h>
#include <QPolygonF>
#include <UIConstants.h>


void WireNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    event->accept();
}

void WireNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << "mouseMoveEvent in WireNode";

    setPos(event->scenePos());

    for (auto *line : lines_) line->rebuild();
}

WireNode::WireNode(WireItem *parent_wire) : WireEndPoint(parent_wire) {
    setZValue(NodeZValue);
}


void WireNode::addLine(WireLine *line) {
    lines_.insert(line);
}

void WireNode::removeLine(WireLine *line) {
    lines_.erase(line);
}

std::unordered_set<WireLine *> WireNode::lines() {
    return lines_;
}

QRectF WireNode::boundingRect() const {
    return {0, 0, width_, height_}; // !!!
}

void WireNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen;
    // pen.setColor(QColorConstants::Black.);

    painter->drawEllipse(QRectF{0, 0, width_, height_});
}

