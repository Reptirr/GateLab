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

void WireItem::removeNode(WireNode *node) {
    assert(node->lines().size() == 2); // because we can remove node only between 2 lines and combine they

    auto lines = node->lines();
    auto it = lines.begin();
    const auto line1 = *it;
    const auto line2 = *++it;

    auto *point1 = line1->from() != node ? line1->from() : line1->to(); // if line1::from is not deleting node => other is
    auto *point2 = line2->from() != node ? line2->from() : line2->to();

    delete node; // there is no reason to have it
    end_points_.erase(point2); // because it is now disconnect from wire graph

    createLine(point1, point2);
}

void WireItem::removeEndPoint(WireEndPoint *end_point) {
    assert(end_points_.contains(end_point));

    for (WireLine *line : end_point->lines()) {
        line->removeWireEndpoint();
        end_point->removeLine(line);
    }

    end_points_.erase(end_point);

    if (end_points_.size() <= 1) {
        delete this; // there is no reason to be
    }
}

void WireItem::createLine(WireEndPoint *from, WireEndPoint *to) {
    assert(end_points_.contains(from) && !end_points_.contains(to));

    end_points_.insert(to);

    auto *line = new WireLine(from, to, this);

    from->addLine(line);
    to->addLine(line);
}

QColor WireItem::color() const {
    return color_;
}

QRectF WireItem::boundingRect() const {
    return childrenBoundingRect();
}
void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}

int WireItem::type() const {
    return WireType;
}

WireItem::~WireItem() {
    for (auto *end_point : end_points_) {
        if (const auto node = dynamic_cast<WireNode *>(end_point)) {
            delete node;
        }
    }
}
