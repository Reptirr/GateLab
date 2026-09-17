#include <my_assert.h>
#include <WireItem.h>

#include <PinItem.h>
#include <WireEndpoint.h>
#include <WireLine.h>
#include <WireNode.h>

void WireItem::setColorBySignal(bool signal = false) {
    color_ = signal ? QColorConstants::Green : QColorConstants::Black;
    update();
}

void WireItem::notifyEndPointDelete() const {
    if (end_points_.size() <= 1) {
        // delete wire if it is only 1 end_point. logic pin is already need be deleted at that moment
        delete this;
    }
}

WireItem::WireItem(WireEndPoint *end_point1, WireEndPoint *end_point2) {
    my_assert(end_point1->scene() == end_point2->scene());

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

void WireItem::collapseNode(WireNode *node) {
    my_assert(node->lines().size() == 2); // because we can remove node only between 2 lines and combine they

    auto lines = node->lines();
    auto it = lines.begin();
    const auto line1 = *it;
    const auto line2 = *++it;

    auto *point1 = line1->from() != node ? line1->from() : line1->to(); // if line1::from is not deleting node => other is
    auto *point2 = line2->from() != node ? line2->from() : line2->to();

    // we cant call removeEndPoint because it will call notifyEndPointDelete and wire_item will delete
    delete node;
    end_points_.erase(node); // we deleted & disconnected it
    end_points_.erase(point2); // because it is now disconnect from wire graph

    createLine(point1, point2);
}

void WireItem::removeEndPoint(WireEndPoint *end_point) {
    if (!end_points_.contains(end_point)) return;

    end_point->clearLines();

    end_points_.erase(end_point);

    notifyEndPointDelete();
}

void WireItem::createLine(WireEndPoint *from, WireEndPoint *to) {
    my_assert(end_points_.contains(from) && !end_points_.contains(to));

    end_points_.insert(to);

    auto *line = new WireLine(from, to, this);

    from->addLine(line);
    to->addLine(line);
}

QColor WireItem::color() const {
    return color_;
}

bool WireItem::empty() const {
    if (end_points_.size() == 0) return true;
    if (end_points_.size() > 1) return false;

    auto *end_point = *end_points_.begin();
    if (auto pin = dynamic_cast<PinItem *>(end_point)) {
        return false;
    }
    return true;
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
    qDebug() << "wire_item delete";

    for (auto *end_point : end_points_) {
        if (auto *pin = dynamic_cast<PinItem *>(end_point)) {
            pin->clearLines();
        } else {
            delete end_point;
        }
    }
}
