#pragma once
#include <PinItem.h>
#include <qcolor.h>
#include <QGraphicsItem>


class WireLine;

class WireNode : public QGraphicsItem {

    qreal width_ = 10;
    qreal height_ = 10;

    QColor color_;

    WireLine *behind_{};
    WireLine *ahead_{};

public:
    WireNode() = default; // need call init before using
    WireNode(WireLine *behind, WireLine *ahead) : behind_(behind), ahead_(ahead) {}
    void init(WireLine *behind, WireLine *ahead) {
        behind_ = behind;
        ahead_ = ahead;
    }

    WireLine* behind() const {
        return behind_;
    }
    WireLine* ahead() const {
        return ahead_;
    }

    QRectF boundingRect() const override {
        return {0, 0, width_, height_};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QPen pen;
        pen.setColor(color_);

        painter->setPen(pen);

        if (isUnderMouse()) {
            painter->drawEllipse(boundingRect().adjusted(-3, -3, 3, 3));
        } else {
            painter->drawEllipse(boundingRect());
        }
    }
};

class WireLine : public QGraphicsItem {
    QGraphicsItem *from_;
    QGraphicsItem *to_;

    QLineF line_{};
    QColor color_{};

public:
    WireLine(QGraphicsItem *item1, QGraphicsItem *item2)  {
        from_ = item1;
        to_ = item2;
        line_ = {item1->pos(), item2->pos()};
    }

    QLineF line() const {
        return line_;
    }
    QGraphicsItem* from() const {
        return from_;
    }
    QGraphicsItem* to() const {
        return to_;
    }

    void rebuild() {
        line_ = {from_->pos(), to_->pos()};
        update();
    }

    QPainterPath shape() const override {
        QPainterPath path;

        path.moveTo(line_.p1());
        path.lineTo(line_.p2());

        QPainterPathStroker stroker;

        stroker.setCapStyle(Qt::PenCapStyle::RoundCap);
        stroker.setWidth(3);

        return stroker.createStroke(path);
    }

    QRectF boundingRect() const override {
        return shape().boundingRect();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QPen pen;
        pen.setColor(color_);
        pen.setWidth(3);

        painter->setPen(pen);

        painter->drawLine(line_);
    }
};


// for unordered_set

struct WireLineHash {
    size_t operator()(const WireLine* line) const {
        return std::hash<QGraphicsItem*>()(line->from()) ^ std::hash<QGraphicsItem*>()(line->to());
    }
};

struct WireLineEqual {
    bool operator()(const WireLine* line1, const WireLine* line2) const {
        return line1->from() == line2->from() && line1->to() == line2->to() && line1->line() == line2->line();
    }
};