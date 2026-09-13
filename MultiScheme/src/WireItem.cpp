#include <WireItem.h>

#include <PinItem.h>
#include <WireEndpoint.h>
#include <WireLine.h>
#include <WireNode.h>

void WireItem::setColorBySignal(bool signal = false) {
    color_ = signal ? QColorConstants::Green : QColorConstants::Black;
    update();
}

WireItem::WireItem(WireEndPoint *end_point1, WireEndPoint *end_point2) {
    assert(end_point1->scene() == end_point2->scene());

    setZValue(WireZValue);

    end_point1->scene()->addItem(this); // need it because WireLine need be on scene in constructor

    auto *line = new WireLine(end_point1, end_point2, this);

    end_point1->addLine(line);
    end_point2->addLine(line);

    end_points_.insert({end_point1, end_point2});

    end_point1->scene()->addItem(line);
}

void WireItem::createNode(const WireLine *on_line, const QPointF pos) {
    // slice line to 2 ones and node between they

    auto *from = on_line->from();
    auto *to = on_line->to();
    delete on_line; // remove recordings about old line in endpoints & delete it

    auto *node = new WireNode(this);
    node->setPos(pos);

    auto *line1 = new WireLine(from, node, this);
    auto *line2 = new WireLine(node, to, this);
    from->addLine(line1);
    node->addLine(line1);
    node->addLine(line2);
    to->addLine(line2);

    end_points_.insert(node);
}

void WireItem::removeEndPoint(WireEndPoint *end_point) {
    assert(end_points_.contains(end_point));

    for (WireLine *line :
        end_point->lines()) {
        line->removeWireEndpoint();
        end_point->removeLine(line);
    }

    end_points_.erase(end_point);
}

QColor WireItem::color() const {
    return color_;
}

QPainterPath WireItem::shape() const {
    // QPainterPath path{};
    //
    // std::unordered_set<WireLine*> lines;
    // for (const auto child : childItems()) {
    //     path += child->shape();
    // }
    //
    // // расширяем путь что бы допустить промахи
    // QPainterPathStroker stroker{};
    //
    // stroker.setWidth(7);
    // stroker.setCapStyle(Qt::RoundCap);   // закругленные края
    // stroker.setJoinStyle(Qt::RoundJoin); // закругленные изломы

    return {};
}

QRectF WireItem::boundingRect() const {
    return shape().boundingRect();
}
void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}

int WireItem::type() const {
    return WireType;
}
