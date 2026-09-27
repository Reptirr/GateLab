#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <unordered_set>


class WireItem;
class WireLine;
class WireNode;
class WireEndPoint;
class PinItem;
class LogicWire;

struct WireDivideResult {
    std::unordered_set<WireItem *> new_wires{};
    bool is_source_wire_deleted{};
    std::unordered_set<PinItem *> disconnected_pins{};
};


// always need 2 end points
class WireItem : public QGraphicsItem {
    // with logic side
    QColor color_ = QColorConstants::Black;

    // graph
    std::unordered_set<WireEndPoint *> end_points_;


public:
    WireItem(WireEndPoint *point);

    void divideLine(const WireLine *on_line, QPointF pos); // create node on line

    /// @return is collapsed
    bool collapseNode(WireNode *node);


    void removeEndPoint(WireEndPoint *end_point);

    /// create line from our node to other end_point
    void createLine(WireEndPoint *from, WireEndPoint *to);

    /// delete wire from arg
    /// @return new points from wire to this
    std::unordered_set<WireEndPoint *> uniteWire(WireItem *wire_item, WireNode *line_from, WireNode *line_to);

    WireDivideResult divideWireIn(WireNode *node);

    bool contains(WireEndPoint *end_point) const;

    void setColorBySignal(bool signal);
    QColor color() const;

    int end_points_size() const;
    std::unordered_set<PinItem *> pins() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type() const override;

    ~WireItem() override;
};