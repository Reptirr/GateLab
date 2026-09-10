#include <WireItem.h>

#include <PinItem.h>
#include <WireItemHelpers.h>

void WireItem::setColorBySignal(bool signal) {
    color_ = signal ? QColorConstants::Green : QColorConstants::Black;
    update();
}

WireItem::WireItem(QGraphicsItem *from, QGraphicsItem *to) {
    auto *wire_line = new WireLine{from, to};

    lines_.insert(wire_line);
}

void WireItem::addNode(WireLine *on_line, const QPointF pos) {
    assert(on_line->contains(pos));

    // разделяем линию на 2 части, между которыми нода
    // удаляем текущую линию, создаем 2 новых линии с соединением в ноде

    lines_.erase(on_line);

    auto *node = new WireNode();

    auto *line_before = new WireLine(on_line->from(), node);
    auto *line_after = new WireLine(node, on_line->to());

    node->init(line_before, line_after);

    delete on_line;

    // добавляем всех на сцену
    scene()->addItem(line_before);
    scene()->addItem(node);
    scene()->addItem(line_after);

    lines_.insert({line_before, line_after});
}

void WireItem::removeNode(const WireNode *node) {
    // убираем текущую ноду и соединяем линии которые были соединены с нодой напрямую

    auto *new_line = new WireLine(node->behind()->from(), node->ahead()->to());

    delete node->behind();
    delete node->ahead();
    delete node;

    scene()->addItem(new_line);

    lines_.insert(new_line);
}

void WireItem::moveNode(WireNode *node, const QPointF to_pos) {
    node->setPos(to_pos);

    node->behind()->rebuild();
    node->ahead()->rebuild();
}

QPainterPath WireItem::shape() const {
    QPainterPath path{};

    for (const auto &line : lines_) {
    //     path.moveTo(line.p1());
    //     path.lineTo(line.p2());
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
