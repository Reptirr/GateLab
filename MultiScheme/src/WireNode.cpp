#include <qgraphicssceneevent.h>
#include <QPainter>
#include <WireLine.h>
#include <WireNode.h>
#include <QPolygonF>
#include <UIConstants.h>


void WireNode::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    move_helper_.onMousePress(event->scenePos());
    event->accept();
}

void WireNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    move(move_helper_.onMouseMove(scenePos(), event->scenePos()));
}

WireNode::WireNode(WireItem *parent_wire) : WireEndPoint(parent_wire) {
    setZValue(NodeZValue);
}

WireNode::WireNode() {
    setZValue(NodeZValue);
}


void WireNode::addLine(WireLine *line) {
    lines_.insert(line);
}

void WireNode::removeLine(WireLine *line) {
    if (process_line_ == line) return;
    lines_.erase(line);
}

void WireNode::clearLines() {
    for (auto *line : lines_) {
        process_line_ = line;
        line->removeWireEndpoint();
    }
    lines_.clear();
}

void WireNode::disconnect() {
    wire_item_ = nullptr;
    delete this;
}

void WireNode::move(const QPointF pos) {
    setPos(pos);
    for (auto *line : lines_) line->rebuild();
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
    WireNode::clearLines();
}

