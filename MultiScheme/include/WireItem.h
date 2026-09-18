#pragma once
#include <QGraphicsItem>
#include <QPainter>


class WireLine;
class WireNode;
class WireEndPoint;
class PinItem;
class LogicWire;


// always need 2 end points
class WireItem : public QGraphicsItem {
    // with logic side
    QColor color_ = QColorConstants::Black;

    void setColorBySignal(bool signal);

    std::shared_ptr<LogicWire> logic_wire_;
    friend LogicWire;

    // graph
    std::unordered_set<WireEndPoint *> end_points_;

    /// @return is wire deleted
    bool notifyEndPointDelete() const;

    friend WireNode;

public:
    WireItem(WireEndPoint *end_point1, WireEndPoint *end_point2, std::shared_ptr<LogicWire> logic_wire);

    void divideLine(const WireLine *on_line, QPointF pos); // create node on line

    /// @return is collapsed
    bool collapseNode(WireNode *node); // remove node and combine 2 lines to 1
    /// @return is wire deleted
    bool removeEndPoint(WireEndPoint *end_point);

    void createLine(WireEndPoint *from, WireEndPoint *to); // create line from our node to other end_point

    /// delete wire from arg
    /// @return new points from wire to this
    std::unordered_set<WireEndPoint *> uniteWire(WireItem *wire_item, WireNode *line_from, WireNode *line_to);

    QColor color() const;
    bool empty() const;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type() const override;

    ~WireItem() override;
};