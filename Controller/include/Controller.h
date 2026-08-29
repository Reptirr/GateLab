#pragma once
#include <LogicPin.h>
#include <LogicTransistor.h>
#include <LogicWire.h>
#include <MainView.h>
#include <PinItem.h>
#include <unordered_map>
#include <WireItem.h>

class LogicTransistor;
class LogicWire;
class WireItem;
class LogicPin;


class Controller {
    MainView *main_view_{};

    std::unordered_map<PinItem*, LogicPin*> pins_;
    std::unordered_map<WireItem*, LogicWire*> wires_;
    std::unordered_map<ComponentItem*, LogicTransistor*> black_box_components_;

    std::vector<ComponentItem*> drillable_components_;

public:
    Controller() {
        main_view_ = new MainView(new QGraphicsScene());
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

    void addTransistor(ComponentItem* component) {
        // component must has no drilling
        if (component->interior() != nullptr) return;

        // component

        // =========
        // UI
        // =========

        // get pins from component
        auto pin_items = component->pins();

        // =========
        // LOGIC
        // =========

        // create transistor
        auto *transistor = new LogicTransistor();

    }
};
