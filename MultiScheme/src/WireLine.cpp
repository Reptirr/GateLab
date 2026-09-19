#include <QPainter>
#include <Utils.h>
#include <WireEndpoint.h>
#include <WireLine.h>
#include <QGraphicsSceneHoverEvent>
#include <WireItem.h>
#include <UIConstants.h>

WireLine::WireLine(WireEndPoint *from, WireEndPoint *to, WireItem *parent_wire) : QGraphicsItem(parent_wire), from_(from),
    to_(to), parent_wire_(parent_wire) {
    qDebug() << "wireline constructor. wire_item: " << parent_wire_;

    setZValue(LineZValue);
    setAcceptHoverEvents(true);

    line_ = {
        parent_wire->mapFromItem(from, from->boundingRect().center()),
        parent_wire->mapFromItem(to, to->boundingRect().center())
    };

    romb_ = new QGraphicsPolygonItem{{}, this};
    romb_->setVisible(false);

    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColorConstants::DarkGreen);

    romb_->setPen(pen);

    constexpr QRectF rect{0, 0, 10, 15};
    constexpr qreal cx = rect.center().x();
    constexpr qreal cy = rect.center().y();

    romb_->setPolygon(QPolygonF{
        QPointF(cx, rect.top()),
        QPointF(rect.right(), cy),
        QPointF(cx, rect.bottom()),
        QPointF(rect.left(), cy)
    });
}

WireEndPoint * WireLine::from() const {
    return from_;
}

WireEndPoint * WireLine::to() const {
    return to_;
}

WireItem * WireLine::wire() const {
    return parent_wire_;
}

void WireLine::setParentWire(WireItem *wire_item) {
    parent_wire_ = wire_item;
}

void WireLine::rebuild() {
    prepareGeometryChange();

    line_ = {
        parent_wire_->mapFromItem(from_, from_->boundingRect().center()),
        parent_wire_->mapFromItem(to_, to_->boundingRect().center())
    };

    update();
}

QPainterPath WireLine::shape() const {
    QPainterPath path;

    path.moveTo(line_.p1());
    path.lineTo(line_.p2());

    QPainterPathStroker stroker;
    stroker.setWidth(3);
    stroker.setCapStyle(Qt::FlatCap);

    return stroker.createStroke(path);
}

QRectF WireLine::boundingRect() const {
    return shape().boundingRect();
}

void WireLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(parent_wire_->color());

    painter->setPen(pen);

    painter->drawLine(line_);
}

void WireLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    const auto closest_pos = closestPointOnLine(line_, event->pos());

    romb_->setPos({
        closest_pos - romb_->boundingRect().center()
    });
    romb_->setVisible(true);
}

void WireLine::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    const auto closest_pos = closestPointOnLine(line_, event->pos());

    romb_->setPos({
        closest_pos - romb_->boundingRect().center()
    });
}

void WireLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    romb_->setVisible(false);
}

void WireLine::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    event->accept();
    ungrabMouse();
    parent_wire_->divideLine(this, event->scenePos()); // after that this is deleting
}


void WireLine::removeWireEndpoint() const {
    delete this;
}

WireLine::~WireLine() {
    from_->removeLine(this);
    to_->removeLine(this);
}
