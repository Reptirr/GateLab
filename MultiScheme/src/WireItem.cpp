#include <LogicWire.h>
#include <my_assert.h>
#include <WireItem.h>

#include <PinItem.h>
#include <Utils.h>
#include <WireEndpoint.h>
#include <WireLine.h>
#include <WireNode.h>

WireItem::WireItem(WireEndPoint *point) {
    setZValue(WireZValue);

    point->setParentWire(this);
    end_points_.insert(point);
}


void WireItem::divideLine(const WireLine *on_line, const QPointF pos) {
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

bool WireItem::collapseNode(WireNode *node) {
    if (node->lines().size() != 2) return false;

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

    return true;
}

void WireItem::removeEndPoint(WireEndPoint *end_point) {
    my_assert(end_points_.contains(end_point));

    end_point->clearLines();

    end_points_.erase(end_point);

    end_point->disconnect();
}

void WireItem::createLine(WireEndPoint *from, WireEndPoint *to) {
    my_assert(end_points_.contains(from) && !end_points_.contains(to));

    end_points_.insert(to);

    auto *line = new WireLine(from, to, this);

    from->addLine(line);
    to->addLine(line);

    from->setParentWire(this);
    to->setParentWire(this);

    if (instanceOfWireNode(from)) from->setParentItem(this);
    if (instanceOfWireNode(to)) to->setParentItem(this);
}

std::unordered_set<WireEndPoint *> WireItem::uniteWire(WireItem *wire_item, WireNode *line_from, WireNode *line_to) {
    my_assert(wire_item != this);
    my_assert(end_points_.contains(line_from)); // other wire contains node from we create connection
    my_assert(wire_item->end_points_.contains(line_to)); // we contains node to we create connection

    // gets points from arg wire, insert to our and create line between nodes from args

    // create line
    createLine(line_from, line_to);

    std::unordered_set<WireEndPoint *> new_end_points{wire_item->end_points_.begin(), wire_item->end_points_.end()};
    for (auto *new_end_point : new_end_points) {
        new_end_point->setParentWire(this);

        if (!instanceOfPinItem(new_end_point)) // we change ownership only for nodes and lines
            new_end_point->setParentItem(this);

        for (auto *line : new_end_point->lines()) {
            line->setParentWire(this);
            line->setParentItem(this);
        }

        end_points_.insert(new_end_point);
    }

    wire_item->end_points_.clear();
    delete wire_item;

    return new_end_points;
}

void process_point(WireLine *previous_line, WireEndPoint *point, std::unordered_set<WireEndPoint *> &new_points) {
    if (instanceOfPinItem(point)) new_points.insert(point);
    else /* WireNode */ {
        new_points.insert(point);

        for (auto *line : point->lines()) {
            if (line == previous_line) continue;

            auto next_point = line->to() == point ? line->from() : line->to();
            process_point(line, next_point, new_points);
        }
    }
}

WireDivideResult WireItem::divideWireIn(WireNode *node) {
    my_assert(contains(node));

    std::vector< std::unordered_set<WireEndPoint *> > other_points{};
    std::unordered_set<WireEndPoint *> our_points{};

    // one of line is our wire, other is new wires
    // node will delete

    int i = 0;
    for (auto *line : node->lines()) {
        auto next_point = line->to() == node ? line->from() : line->to();

        if (i == 0) {
            process_point(line, next_point, our_points);
        } else {
            other_points.emplace_back();

            process_point(line, next_point, other_points[i-1]);
        }

        // also remove line-connection
        next_point->removeLine(line);
        node->removeLine(line);
        delete line;

        i++;
    }

    end_points_.erase(node);
    delete node; // node must be has no recordings about neighbours & lines have no recordings about node

    WireDivideResult res{};

    // check is our wire is useless
    if (our_points.size() < 2) {
        auto *point = *our_points.begin();

        if (auto *pin = instanceOfPinItem(point)) {
            res.disconnected_pins.insert(pin);
        } else /* WireNode */ {
            my_assert(point->lines().size() == 0); // we already remove line on node deleting

            end_points_.erase(point);
            our_points.erase(point);

            delete point;
        }

        // set flag of are we deleted
        res.is_source_wire_deleted = true;
    }

    // we have sets of new wires
    for (auto points_set : other_points) {
        // delete useless wires in new
        if (points_set.size() < 2) {
            if (auto *pin = instanceOfPinItem(*points_set.begin())) {
                res.disconnected_pins.insert(pin);
            }
            else {
                auto *point = *points_set.begin();

                point->clearLines();
                point->disconnect();

                end_points_.erase(point);
                delete point;
            }

            continue;
        }


        auto *new_wire = new WireItem(*points_set.begin());
        scene()->addItem(new_wire);
        res.new_wires.insert(new_wire);

        for (auto *point : points_set) {
            for (auto *line : point->lines()) {
                line->setParentWire(new_wire);
                line->setParentItem(new_wire);
            }

            if (instanceOfWireNode(point)) {
                point->setParentWire(new_wire);
                point->setParentItem(new_wire);
            }

            end_points_.erase(point);
            new_wire->end_points_.insert(point);
        }
    }

    end_points_ = our_points;

    return res;
}

bool WireItem::contains(WireEndPoint *end_point) const {
    return end_points_.contains(end_point);
}

void WireItem::setColorBySignal(bool signal = false) {
    color_ = signal ? QColorConstants::Green : QColorConstants::Black;
    update();
}

QColor WireItem::color() const {
    return color_;
}

int WireItem::end_points_size() const {
    return end_points_.size();
}

std::unordered_set<PinItem *> WireItem::pins() const {
    std::unordered_set<PinItem *> res{};

    for (auto *point : end_points_) {
        if (auto *pin = instanceOfPinItem(point))
            res.insert(pin);
    }

    return res;
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
