#pragma once
#include <LogicPin.h>
#include <LogicTransistor.h>
#include <LogicWire.h>
#include <MainView.h>
#include <PinItem.h>
#include <TransistorItem.h>
#include <unordered_map>
#include <WireItem.h>
#include <QObject>

class LogicTransistor;
class LogicWire;
class WireItem;
class LogicPin;


class Controller : public QObject {
    Q_OBJECT

    MainView *main_view_{};

    std::unordered_map<PinItem*, LogicPin*> pins_;
    std::unordered_map<WireItem*, LogicWire*> wires_;
    std::unordered_map<ComponentItem*, LogicTransistor*> black_box_components_;

    std::vector<ComponentItem*> drillable_components_;



public slots:
    void onCreateTransistorRequest(QPointF pos) {
        auto size = TransistorItem{{0,0}}.boundingRect().size();

        pos.setX(pos.x() - size.width()/2);
        pos.setY(pos.y() - size.height()/2);

        addTransistor(new TransistorItem(pos));
    }

    void onWireCreateRequest(PinItem *pin1, PinItem *pin2) {
        addWire(pin1, pin2);
    }


public:
    Controller() {
        main_view_ = new MainView(new QGraphicsScene());

    }

    MainView* mainView() {
        return main_view_;
    }

    void addWire(PinItem* pin1, PinItem* pin2) {
        // pins must exist, be registered and share the same (non-null) scene
        if (!pin1 || !pin2) return;
        if (!pins_.contains(pin1) || !pins_.contains(pin2)) return;
        if (!pin1->scene() || pin1->scene() != pin2->scene()) return;

        // make wire
        auto *wire_item = new WireItem();
        auto *wire_logic = new LogicWire();

        // =========
        // UI
        // =========

        // add pins to item
        wire_item->addPin(pin1);
        wire_item->addPin(pin2);

        // add wire to scene
        qDebug() << "add item at addWire (add wire to scene)";
        main_view_->scene()->addItem(wire_item);

        // =========
        // LOGIC
        // =========

        // add wire to pins in logic
        pins_.at(pin1)->addHandler(wire_logic);
        pins_.at(pin2)->addHandler(wire_logic);

        // add pins to wire in logic
        wire_logic->addPin(pins_.at(pin1));
        wire_logic->addPin(pins_.at(pin2));


        // add wire to registry
        wires_.insert({wire_item, wire_logic});
    }

    void addTransistor(TransistorItem* component) {
        // component must has no drilling
        if (component->interior() != nullptr) return;

        // pins must be 3
        if (component->pins().size() != 3) return;


        // =========
        // UI
        // =========

        // add component to scene
        qDebug() << "add item at add transistor (add component to scene)";
        main_view_->scene()->addItem(component);

        // get pins from component
        auto &pin_items = component->pins();

        // add pins item to scene
        for (auto *pin : pin_items) {
            qDebug() << "add item at addTransistor (add pin to scene)";
            main_view_->scene()->addItem(pin);
        }


        // =========
        // LOGIC
        // =========

        // create logic pins
        auto *left = new LogicPin();
        auto *top = new LogicPin();
        auto *right = new LogicPin();

        // create transistor
        auto *transistor = new LogicTransistor(left, top, right);


        // add transistor to registry
        black_box_components_.insert({component, transistor});

        // add pins to registry
        pins_.insert({pin_items[0], left});
        pins_.insert({pin_items[1], top});
        pins_.insert({pin_items[2], right});
    }
};
