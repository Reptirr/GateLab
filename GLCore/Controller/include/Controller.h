#pragma once

#include <QMouseEvent>
#include <InputMapper.h>
#include <LogicComponentsFactory.h>
#include <LogicController.h>
#include <LogicPin.h>
#include <LogicWire.h>
#include <MainView.h>
#include <PinItem.h>
#include <instant/TransistorItem.h>
#include <unordered_map>
#include <WireItem.h>
#include <QObject>
#include <UIController.h>

class QMainWindow;
class LogicTransistor;
class LogicWire;
class WireItem;
class LogicPin;

/*
 * Ownership model:
 * Component has: pins and item
 * Pin has: owner, wire
 * Wire has: pins and item(color changing)
 *
 * Controller working about remove all from all
 *
 * Wire does not delete anything
 *
 * on remove firstly delete logic, after ui
 * on create firstly create ui after logic
 *
 * if on ui wire is no pins then logic wire deletes. if get request to connect pin to wire we re-create logic wire
 *
 */

class Controller : public QObject {
    Q_OBJECT

    std::unordered_map<PinItem*, std::weak_ptr<LogicPin>> pins_;
    std::unordered_map<WireItem*, std::weak_ptr<LogicWire>> wires_;
    std::unordered_map<ComponentItem*, std::weak_ptr<LogicComponent>> black_box_components_;

    UIController ui_controller_;
    LogicController logic_controller_{};

public slots:
    void onComponentCreateRequest(ComponentItem *component_item) {
        ui_controller_.createComponent(component_item);
        auto logic_component = logic_controller_.createComponent(logicTypeByItem(component_item));

        black_box_components_.insert({component_item, logic_component});
        for (int i = 0; i < component_item->pins().size(); i++) {
            pins_.insert({component_item->pins()[i], logic_component.lock()->pins()[i]});
        }
    }
    void onComponentRemoveRequest(ComponentItem *component_item) {
        // delete now because ui_controller_::removeComponent delete component_item
        for (auto *pin_item : component_item->pins())
            pins_.erase(pin_item);

        logic_controller_.removeComponent(black_box_components_.at(component_item));
        auto deleted_wires = ui_controller_.removeComponent(component_item);

        /*
         * ui_wire deleting if it has 1 and less points. logic wire auto-deleting if it has 0 connected pins.
         * we sync it. if ui is deleted, logic also
         */
        for (auto *wire_item : deleted_wires) {
            auto logic_wire__ = wires_.extract(wire_item).mapped();

            // imitate legal deleting
            if (auto logic_wire = logic_wire__.lock()) {
                for (auto pin__ : logic_wire->pins()) {
                    logic_wire__.lock()->setSignalConsumer(nullptr); // because there is no ui wire. !костыль!
                    logic_controller_.disconnectPin(logic_wire__, pin__);
                }
            }
        }

        black_box_components_.erase(component_item);
    }


    // =========
    // WIRING
    // =========

    void onWireCreateRequest(WireEndPoint *point1, WireEndPoint *point2) {
        auto *pin_item1 = dynamic_cast<PinItem *>(point1);
        auto *pin_item2 = dynamic_cast<PinItem *>(point2);

        if (pin_item1) my_assert(pins_.contains(pin_item1));
        if (pin_item2) my_assert(pins_.contains(pin_item2));

        std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin>> logic_pins{};

        if (pin_item1) logic_pins.insert(pins_.at(pin_item1));
        if (pin_item2) logic_pins.insert(pins_.at(pin_item2));

        auto *wire_item = ui_controller_.createWire(point1, point2);
        std::weak_ptr<LogicWire> logic_wire{};

        if (logic_pins.size() != 0) {
            logic_wire = logic_controller_.createWire(logic_pins, wire_item);
        }

        wires_.insert({wire_item, logic_wire});
    }
    void onAddPinToWireRequest(WireItem *wire_item, WireEndPoint *from, PinItem *pin_item) {
        my_assert(wires_.contains(wire_item));
        my_assert(pins_.contains(pin_item));
        my_assert(from->parentWire() == wire_item);
        my_assert(wire_item->end_points_size() > 1); // else somewhere we have miss delete empty(size < 2) wire

        auto &logic_wire = wires_.at(wire_item);
        auto logic_pin = pins_.at(pin_item);

        if (logic_wire.expired())
            logic_wire = logic_controller_.createWire({logic_pin}, wire_item);
        else
            logic_controller_.connectPin(logic_wire, logic_pin);

        ui_controller_.connectPinToWire(wire_item, from, pin_item);
    }
    void onRemovePinFromWireRequest(WireItem *wire_item, PinItem *pin_item) {
        my_assert(wires_.contains(wire_item));
        my_assert(pins_.contains(pin_item));
        my_assert(wire_item->end_points_size() > 1); // else somewhere we have miss delete empty(size < 2) wire

        // LOGIC

        auto logic_wire = wires_.at(wire_item);
        my_assert(!logic_wire.expired()); // cant remove pin from non-exists logic wire
        auto logic_pin = pins_.at(pin_item);
        my_assert(!logic_pin.expired());

        logic_controller_.disconnectPin(logic_wire, logic_pin);

        // UI

        if (ui_controller_.disconnectPinFromWire(wire_item, pin_item)) {
            // wire_item is invalid
            wires_.erase(wire_item);
        }
    }
    void onCreateNodeInLine(WireLine *line_item, QPointF pos) {
        ui_controller_.divideLineInNode(line_item, pos);
    }
    void onCreateLineToEndPoint(WireItem *wire_item, WireEndPoint *from, WireEndPoint *to) {
        ui_controller_.createLine(wire_item, from, to);
    }
    void onCreateLineToPos(WireItem *wire_item, WireEndPoint *from, QPointF pos, WireNode *&new_node) {
        new_node = ui_controller_.createNode(wire_item, from, pos);
    }
    void onCollapseNode(WireItem *wire_item, WireNode *node) {
        ui_controller_.collapseNode(wire_item, node);
    }
    void onDivideWireInNode(WireItem *wire_item, WireNode *node) {
        auto res = ui_controller_.divideWireInNode(wire_item, node);
        std::unordered_set<PinItem *> disconnected_pins = res.disconnected_pins;
        std::unordered_set<WireItem *> new_wire_items = res.new_wires;

        // in ui already all divided

        // reconnect pins
        for (auto *new_wire_item : new_wire_items) {
            std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin>> logic_pins{};

            for (auto *pin_item : new_wire_item->pins()) {
                auto logic_pin__ = pins_.at(pin_item);

                // disconnect logic pin
                wires_.at(wire_item).lock()->removePin(logic_pin__);
                logic_pin__.lock()->removeWire();

                logic_pins.insert(logic_pin__);
            }

            // create new logic wire with pins
            std::weak_ptr<LogicWire> logic_wire{};
            if (logic_pins.size() > 0)
                logic_wire = logic_controller_.createWire(logic_pins, new_wire_item);

            wires_.insert({new_wire_item, logic_wire});
        }

        // disconnect pins
        for (auto *pin_item : disconnected_pins) {
            auto logic_pin__ = pins_.at(pin_item);

            logic_pin__.lock()->wire().lock()->removePin(logic_pin__);
            logic_pin__.lock()->removeWire();
        }

        // delete source wire if it is
        if (res.is_source_wire_deleted) {
            delete wire_item;
            // logic wire already reconnected/disconnected

            wires_.erase(wire_item);
        }
    }
    void onUniteWire(WireItem *first, WireItem *second, WireNode *from, WireNode *to) {
        my_assert(wires_.contains(first));
        my_assert(wires_.contains(second));
        my_assert(first != second);

        // NOTE: first in logic and first in ui must be same. it is need for successful signalConsumer unite

        if (wires_.at(first).expired() && !wires_.at(second).expired()) {
            // we move logic_wire from second to first recording
            wires_[first] = wires_[second];
            wires_[first].lock()->setSignalConsumer(first);
        } else if (!wires_.at(first).expired() && wires_.at(second).expired()) {
            // we don`t do anything
        } else if (wires_.at(first).expired() && wires_.at(second).expired()) {
            // we don`t do anything
        } else {
            // we unite logic
            logic_controller_.uniteWire(wires_.at(first), wires_.at(second));
        }

        ui_controller_.uniteWire(first, second, from, to);

        wires_.erase(second);
    }


public:
    explicit Controller(QGraphicsScene *scene) : ui_controller_(scene) {

    }
};
