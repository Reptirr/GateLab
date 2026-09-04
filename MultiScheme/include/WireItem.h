#pragma once
#include <QGraphicsItem>
#include <QPainter>
#include <QLineF>
#include <QPainterPath>
#include <set>
#include <unordered_map>


class PinItem;
class LogicWire;

class WireItem : public QGraphicsItem {
protected:
    std::set<PinItem *> pins_;
    std::unordered_map<PinItem *, QLineF> lines_;
    QPointF lines_center_{};

private:
    QColor color_ = QColorConstants::Black;

    void setColorBySignal(bool signal);

    // пересчитывает центр (среднее точек всех пинов) и перестраивает
    // линии центр -> каждый пин
    void rebuildLines();

    friend LogicWire;

public:
    WireItem() = default;

    QPainterPath shape() const override;
    QRectF boundingRect() const override;

    bool empty() const;

    void addPin(PinItem *pin);
    void removePin(PinItem *pin);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    int type() const override;
};