#include <WireItem.h>

#include <PinItem.h>

void WireItem::setColorBySignal(bool signal) {
    color_ = signal ? QColorConstants::Green : QColorConstants::Black;
    update();
}

void WireItem::rebuildLines() {
    prepareGeometryChange();

    if (pins_.empty()) {
        lines_.clear();
        lines_center_ = QPointF{};
        return;
    }

    // центр пина в сценовых координатах
    auto pinCenter = [](PinItem *pin) {
        return pin->mapToScene(pin->boundingRect().center());
    };

    QPointF sum{};
    for (const auto pin : pins_)
        sum += pinCenter(pin);
    lines_center_ = sum / static_cast<qreal>(pins_.size());

    lines_.clear();
    for (const auto pin : pins_)
        lines_.emplace(pin, QLineF(lines_center_, pinCenter(pin)));

    update();
}

QPainterPath WireItem::shape() const {
    QPainterPath path{};

    for (const auto &pair : lines_) {
        const auto &line = pair.second;
        path.moveTo(line.p1());
        path.lineTo(line.p2());
    }

    // расширяем путь что бы допустить промахи
    QPainterPathStroker stroker{};

    stroker.setWidth(7);
    stroker.setCapStyle(Qt::RoundCap);   // закругленные края
    stroker.setJoinStyle(Qt::RoundJoin); // закругленные изломы

    return stroker.createStroke(path);
}

QRectF WireItem::boundingRect() const {
    return shape().boundingRect();
}

bool WireItem::empty() const {
    return lines_.empty() && pins_.empty();
}

void WireItem::addPin(PinItem *pin) {
    if (!pins_.insert(pin).second)
        return; // уже был подключён

    rebuildLines();
}

void WireItem::removePin(PinItem *pin) {
    if (pins_.erase(pin) == 0)
        return; // такого пина и не было

    lines_.erase(pin);
    rebuildLines();
}

void WireItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(QPen(color_));
    for (const auto &pair : lines_)
        painter->drawLine(pair.second);
}

int WireItem::type() const {
    return WireType;
}
