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
    // mode switching
    switch (keyEvent->key()) {
        case Qt::Key_W:
            qDebug() << "change mode to wire creating";
            mode_ = WireCreating{};
            return;

        case Qt::Key_C:
            qDebug() << "change mode to component manipulating";
            mode_ = ComponentManipulating{};
            return;

        default: break;
    }

    if (std::holds_alternative<ComponentManipulating>(mode_)) {
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
    }
}

void InputMapper::onMousePress(const QMouseEvent *e) {
    if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
        auto *pin_item = getItem<PinItem*>(e->pos());
        if (pin_item == nullptr) {
            qDebug() << "get mouse press not at pin";
            return;
        }

        // start wire creating
        if (wireCreating->selected_pin == nullptr) {
            qDebug() << "start wire creating";

            wireCreating->selected_pin = pin_item;

            const auto line = QLineF{
                getCenterPos(wireCreating->selected_pin->scenePos(), wireCreating->selected_pin->boundingRect().size()),
                e->pos()
            };

            wireCreating->line_item = new QGraphicsLineItem{line};
            scene_->addItem(wireCreating->line_item);
        }
        // end wire creating
        else {
            qDebug() << "end wire creating";

            emit wireCreateRequest(pin_item, wireCreating->selected_pin);

            // reset mode values
            wireCreating->reset();
        }
    }
}

void InputMapper::onMouseMove(const QMouseEvent *e) const {
    if (const auto wireCreating = std::get_if<WireCreating>(&mode_)) {
        auto *line = wireCreating->line_item;

        if (!line) return; // there is no line to rebuild

        qDebug() << "pin pos: " << wireCreating->selected_pin->scenePos();

        line->setLine(
            QLineF{
                getCenterPos(wireCreating->selected_pin->scenePos(), wireCreating->selected_pin->boundingRect().size()),
                e->pos()
            }
        );
    }
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