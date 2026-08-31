#include <InputMapper.h>
#include <qevent.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <PinItem.h>
#include <ComponentItem.h>

InputMapper::InputMapper(QGraphicsScene *scene) : scene_(scene) {}

void InputMapper::onKeyPress(QKeyEvent *keyEvent, QPointF mousePos) {
    switch (keyEvent->key()) {
        // create transistor
        case Qt::Key_T: {
            emit transistorCreateRequest(mousePos);

            break;
        }

        // create wire
        case Qt::Key_W: {
            qDebug() << "Get w press";

            auto *pin_item = getItem<PinItem*>(mousePos);
            if (pin_item == nullptr) return;

            // logic
            if (selected_pin_ == nullptr) {
                qDebug() << "select pin";
                selected_pin_ = pin_item;
            } else {
                qDebug() << "create wire";
                emit wireCreateRequest(selected_pin_, pin_item);
                selected_pin_ = nullptr;
            }

            break;
        }

        // drill up
        case Qt::Key_Escape: {
            if (drill_stack_.empty()) return;

            auto *next_scene = drill_stack_.top();
            drill_stack_.pop();

            emit drillUpRequest(next_scene);
            break;
        }

        default: break;
    }
}

void InputMapper::onMouseDoubleClick(QMouseEvent *event) {
    auto *component = getItem<ComponentItem*>(event->pos());
    if (component == nullptr || component->interior() == nullptr) return;

    drill_stack_.emplace(scene_);

    emit drillDownRequest(component->interior());
}

void InputMapper::setScene(QGraphicsScene *scene) {
    scene_ = scene;
}
