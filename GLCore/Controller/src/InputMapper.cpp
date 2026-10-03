#include <ComponentCommands.h>
#include <InputMapper.h>



InputMapper::InputMapper(ModeType& mode) : mode_(mode) {

}

void InputMapper::onKeyPress(const QKeyEvent* e, QPointF pos) {
    if (mode_ == ModeType::COMPONENTS) {
        deletes_ = getItem<ComponentItem *>(pos);
    }
}

void InputMapper::onKeyRelease(const QKeyEvent*, QPointF pos) {
    if (mode_ == ModeType::COMPONENTS) {
        if (deletes_ == getItem<ComponentItem *>(pos)) {
            emit command(new ComponentRemoveCommand())
        }
    }
}

void InputMapper::onMouseDoubleClick(const QMouseEvent*) {
}

void InputMapper::onMousePress(const QMouseEvent *e) {
    if (mode_ == ModeType::COMPONENTS) {
        emit command(new ComponentCreateCommand(e->pos()));
    }
}

void InputMapper::onMouseRelease(const QMouseEvent*) {
}

void InputMapper::onMouseMove(const QMouseEvent*) const {
}


//
// void InputMapper::onKeyPress(const QKeyEvent *keyEvent, const QPointF mousePos) {
//     if (mode_ == EditMode::COMPONENTS) {
//         switch (keyEvent->key()) {
//             // create transistor
//             case Qt::Key_T: {
//                 qDebug() << "Get t press";
//                 emit componentCreateRequest(new TransistorItem{createCenterPos(mousePos, TransistorItem{{0,0}}.size())});
//                 break;
//             }
//
//             // create ntransistor
//             case Qt::Key_N: {
//                 qDebug() << "Get n press";
//                 emit componentCreateRequest(new NTransistorItem{createCenterPos(mousePos, NTransistorItem{{0, 0}}.size())});
//                 break;
//             }
//
//             // create source
//             case Qt::Key_S: {
//                 qDebug() << "Get s press";
//                 emit componentCreateRequest(new SourceItem{createCenterPos(mousePos, SourceItem{{0,0}}.size())});
//                 break;
//             }
//
//             // remove component
//             case Qt::Key_D: {
//                 qDebug() << "Get d press";
//                 if (const auto item = getItem<ComponentItem*>(mousePos))
//                     emit componentRemoveRequest(item);
//             }
//
//             default: break;
//         }
//     } else if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
//         switch (keyEvent->key()) {
//             case Qt::Key_Q:
//             case Qt::Key_Escape:
//                 if (wireCreating->wire_item == nullptr) return; // not in creating
//
//                 // remove current node (remove from wire_item itself)
//                 emit divideWireInNode(wireCreating->wire_item, wireCreating->current_node);
//
//                 wireCreating->reset();
//                 break;
//
//             case Qt::Key_D: {
//                 if (wireCreating->wire_item) return; // we cant remove nodes while we create wire
//
//                 auto *node = getItem<WireNode *>(mousePos);
//                 if (node == nullptr) return;
//
//                 auto wire_item = node->parentWire();
//                 my_assert(wire_item != nullptr);
//
//                 emit divideWireInNode(wire_item, node);
//
//
//                 break;
//             }
//
//             default: break;
//         }
//     }
// }
//
// void InputMapper::onMouseDoubleClick(const QMouseEvent*) {
//
// }
//
// void InputMapper::onMousePress(const QMouseEvent *e) {
//     if (mode_ == EditMode::WIRES) {
//         // we firstly get press at PinItem,
//         // after we produce WireNodes of presses, ()
//         // and close creating after press at PinItem
//
//         auto *pin_item = getItem<PinItem *>(e->pos());
//         WireNode *new_node_item = [&]() -> WireNode * {
//             for (auto *item : scene_->items(e->pos())) {
//                 if (auto *node = dynamic_cast<WireNode *>(item); node && node != wireCreating->current_node)
//                     return node;
//             }
//
//             return nullptr;
//         }();
//
//         // start with pin
//         if (pin_item && wireCreating->wire_item == nullptr) {
//             if (pin_item->lines().size()) {
//                 qDebug() << "pin item already has a line (wire)";
//                 return;
//             }
//
//             // create wire_item & node at mousePos
//             auto *node = new WireNode();
//             node->setPos(e->pos() -node->boundingRect().center());
//             scene_->addItem(node);
//
//             emit wireCreateRequest(pin_item, node);
//
//             wireCreating->wire_item = pin_item->parentWire();
//             wireCreating->current_node = node;
//         }
//         // start with node
//         else if (new_node_item && wireCreating->wire_item == nullptr) {
//             wireCreating->wire_item = new_node_item->parentWire();
//             wireCreating->new_node_creating = true;
//             wireCreating->from_node = new_node_item;
//         }
//         // produce nodes
//         else if (!pin_item && !new_node_item && wireCreating->wire_item != nullptr) {
//             emit createNodeRequest(
//                 wireCreating->wire_item,
//                 wireCreating->current_node,
//                 e->pos()-node_rect.center(),
//                 wireCreating->current_node
//             );
//         }
//         // end creating on node
//         else if (new_node_item && new_node_item->parentWire() != wireCreating->wire_item && wireCreating->wire_item != nullptr) {
//             qDebug() << "wire unite";
//
//             emit uniteWireRequest(
//                 wireCreating->wire_item,
//                 new_node_item->parentWire(),
//                 wireCreating->current_node,
//                 new_node_item
//             );
//             emit collapseNode(
//                 wireCreating->wire_item,
//                 wireCreating->current_node
//             );
//
//             wireCreating->reset();
//         }
//         // end creating on pin
//         else if (pin_item && wireCreating->wire_item != nullptr) {
//
//             emit addPinToWireRequest(wireCreating->wire_item, wireCreating->current_node, pin_item);
//             emit collapseNode(wireCreating->wire_item, wireCreating->current_node);
//
//             wireCreating->reset();
//         }
//
//     }
// }
//
// void InputMapper::onMouseRelease(const QMouseEvent *e) {
//     if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
//         if (wireCreating->new_node_creating) {
//             emit createNodeRequest(wireCreating->wire_item, wireCreating->from_node, e->pos()-wireCreating->from_node->boundingRect().center(), wireCreating->current_node);
//
//             // change state from pre-create to normal produce nodes
//             wireCreating->from_node = nullptr;
//             wireCreating->new_node_creating = false;
//         }
//     }
// }
//
// void InputMapper::onMouseMove(const QMouseEvent *e) const {
//     if (mode_ == EditMode::WIRES) {
//         // move node while we create it
//         if (wireCreating->current_node) {
//             wireCreating->current_node->move(e->pos()-wireCreating->current_node->boundingRect().center());
//         }
//     }
// }