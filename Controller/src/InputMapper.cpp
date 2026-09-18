#include <InputMapper.h>
#include <qevent.h>
#include <qgraphicsscene.h>
#include <PinItem.h>
#include <ComponentItem.h>
#include <algorithm>
#include <Controller.h>
#include <my_assert.h>
#include <WireNode.h>
#include <instant/TransistorItem.h>

static QPointF createCenterPos(const QPointF top_left, const QSizeF &rect) {
    return QPointF{
        top_left.x() - rect.width()/2,
        top_left.y() - rect.height()/2
    };
}

static QPointF getCenterPos(const QPointF top_left, const QSizeF &rect) {
    return QPointF{
        top_left.x() + rect.width()/2,
        top_left.y() + rect.height()/2
    };
}



InputMapper::InputMapper(QGraphicsScene *scene, Controller *controller) : scene_(scene) {
    view_ = dynamic_cast<MainView *>(scene->views().at(0));
}

void InputMapper::onKeyPress(const QKeyEvent *keyEvent, const QPointF mousePos) {
    if (std::holds_alternative<ComponentEdit>(mode_)) {
        switch (keyEvent->key()) {
            // create transistor
            case Qt::Key_T: {
                qDebug() << "Get t press";
                emit componentCreateRequest(new TransistorItem{createCenterPos(mousePos, TransistorItem{{0,0}}.size())});
                break;
            }

            // create source
            case Qt::Key_S: {
                qDebug() << "Get s press";
                emit componentCreateRequest(new SourceItem{createCenterPos(mousePos, SourceItem{{0,0}}.size())});
                break;
            }

            // remove component
            case Qt::Key_D: {
                qDebug() << "Get d press";
                if (const auto item = getItem<ComponentItem*>(mousePos))
                    emit componentRemoveRequest(item);
            }

            default: break;
        }
    } else if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
        switch (keyEvent->key()) {
            case Qt::Key_Q:
            case Qt::Key_Escape:
                // remove current node (remove from wire_item itself)
                delete wireCreating->current_node;

                wireCreating->reset();
                break;

            case Qt::Key_D: {
                auto *node = getItem<WireNode *>(mousePos);
                if (node == nullptr) return;

                auto wire_item = node->parentWire();
                if (wire_item == nullptr) {
                    delete node;
                    break;
                }

                // collapse or just remove
                if (!wire_item->collapseNode(node)) {
                    wire_item->removeEndPoint(node);
                }

                break;
            }

            default: break;
        }
    }
}

void InputMapper::onMousePress(const QMouseEvent *e) {
    if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
        // we firstly get press at PinItem,
        // after we produce WireNodes of presses, ()
        // and close creating after press at PinItem

        auto *pin_item = getItem<PinItem *>(e->pos());
        WireNode *new_node_item = [&]() -> WireNode * {
            for (auto *item : scene_->items(e->pos())) {
                if (auto *node = dynamic_cast<WireNode *>(item); node && node != wireCreating->current_node)
                    return node;
            }

            return nullptr;
        }();

        // start create
        if (pin_item && wireCreating->wire_item == nullptr) {
            if (pin_item->lines().size()) {
                qDebug() << "pin item already has a line (wire)";
                return;
            }

            // create wire_item & node at mousePos

            auto *node = new WireNode();
            node->setPos(e->pos()-node->boundingRect().center());
            scene_->addItem(node);

            emit wireCreateRequest(pin_item, node);

            wireCreating->wire_item = pin_item->parentWire();

            node->setParentWire(wireCreating->wire_item);

            wireCreating->last_end_point = pin_item;
            wireCreating->current_node = node;
        }
        // produce nodes
        else if (!pin_item && !new_node_item && wireCreating->wire_item != nullptr) {
            auto *node = new WireNode(wireCreating->wire_item);
            node->setPos(e->pos()-node->boundingRect().center());

            wireCreating->wire_item->createLine(wireCreating->current_node, node);

            wireCreating->last_end_point = wireCreating->current_node;
            wireCreating->current_node = node;
        }
        // end creating on node_item
        else if (new_node_item && new_node_item->parentWire() != wireCreating->wire_item && wireCreating->wire_item != nullptr) {
            qDebug() << "wire unite";

            emit uniteWireRequest(wireCreating->wire_item, new_node_item->parentWire(), wireCreating->current_node, new_node_item);

            new_node_item->parentWire()->collapseNode(wireCreating->current_node);

            wireCreating->reset();
        }
        // end creating on pin_item
        else if (pin_item && wireCreating->wire_item != nullptr) {

            wireCreating->wire_item->createLine(wireCreating->current_node, pin_item);
            wireCreating->wire_item->collapseNode(wireCreating->current_node);

            emit addPinToWireRequest(wireCreating->wire_item, pin_item);

            wireCreating->reset();
        }
        else {
            my_assert(true); // in tests we will check situations when it is execute
        }
    }
}

void InputMapper::onMouseMove(const QMouseEvent *e) const {
    if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
        // move node while we create it
        if (wireCreating->current_node) {
            wireCreating->current_node->move(e->pos()-wireCreating->current_node->boundingRect().center());
        }
    }
}

void InputMapper::onModeChange(const EditMode mode) {
    mode_.emplaceByEnum(mode);
}

void InputMapper::onMouseDoubleClick(const QMouseEvent *event) {
    auto *component = getItem<ComponentItem*>(event->pos());
    if (component == nullptr || component->interior() == nullptr) return;

    // drill_stack_.emplace(scene_);

    emit drillDownRequest(component->interior());
}

void InputMapper::setScene(QGraphicsScene *scene) {
    scene_ = scene;
}