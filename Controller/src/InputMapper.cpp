#include <InputMapper.h>
#include <qevent.h>
#include <qgraphicsscene.h>
#include <PinItem.h>
#include <ComponentItem.h>
#include <algorithm>
#include <Controller.h>
#include <instant/TransistorItem.h>

static QPointF createCenterPos(const QPointF top_left, const QSizeF &rect) {
    return QPointF{
        top_left.x() - rect.width()/2,
        top_left.y() - rect.height()/2
    };
}


InputMapper::InputMapper(QGraphicsScene *scene, Controller *controller) : scene_(scene) {
    view_ = dynamic_cast<MainView *>(scene->views().at(0));
}

void InputMapper::onKeyPress(const QKeyEvent *keyEvent, const QPointF mousePos) {
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

        // remove item
        case Qt::Key_D: {
            qDebug() << "Get d press";

            auto *component = getItem<ComponentItem *>(mousePos);
            if (component == nullptr) return;
            emit componentRemoveRequest(component);
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

void InputMapper::onMousePress(QMouseEvent *e) {
    view_->mousePressEvent(e->clone());
}

void InputMapper::setScene(QGraphicsScene *scene) {
    scene_ = scene;
}
