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
    return QRectF{0, 0, width_, height_}.adjusted(-2, -2, 2, 2);
}

void WireNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QBrush brush;
    brush.setColor(QColorConstants::DarkCyan);
    brush.setStyle(Qt::SolidPattern);

    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);

    painter->drawEllipse(QRectF{0, 0, width_, height_});
}

WireNode::~WireNode() {
    for (const auto *line : lines_) line->removeWireEndpoint();
}

