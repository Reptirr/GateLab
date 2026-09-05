#pragma once
#include <BiMap.h>
#include <InputMapper.h>
#include <LogicComponentsFactory.h>
#include <LogicPin.h>
#include <LogicWire.h>
#include <MainView.h>
#include <PinItem.h>
#include <instant/TransistorItem.h>
#include <unordered_map>
#include <WireItem.h>
#include <QObject>
#include <instant/LogicSource.h>
#include <instant/LogicTransistor.h>
#include <instant/SourceItem.h>

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
 */

class Controller : public QObject {
    Q_OBJECT

    MainView *main_view_{};
    InputMapper *input_mapper_;

    std::unordered_map<PinItem*, LogicPin*> pins_; // for wire creating
    std::unordered_map<WireItem*, LogicWire*> wires_;
    std::unordered_map<ComponentItem*, LogicComponent*> black_box_components_; // for component removing

    std::set<ComponentItem*> drillable_components_;

    void initConnects() {
        // MainView -> InputMapper
        connect(main_view_, &MainView::keyPress,
                input_mapper_, &InputMapper::onKeyPress);
        connect(main_view_, &MainView::mouseDoubleClick,
                input_mapper_, &InputMapper::onMouseDoubleClick);

        // InputMapper -> Controller
        connect(input_mapper_, &InputMapper::drillDownRequest, // drill-down
                this, &Controller::onDrillDownRequest);
        connect(input_mapper_, &InputMapper::drillUpRequest, // drill-up
                this, &Controller::onDrillUpRequest);
        connect(input_mapper_, &InputMapper::wireCreateRequest, // wire-create
                this, &Controller::onWireCreateRequest);
        connect(input_mapper_, &InputMapper::componentRemoveRequest, // component-delete
                this, &Controller::onComponentRemoveRequest);
        connect(input_mapper_, &InputMapper::componentCreateRequest,
                this, &Controller::onComponentCreateRequest); // component-create
    }

public slots:
    void onComponentCreateRequest(ComponentItem *component_item) {
        addComponent(component_item);
    }

    void onWireCreateRequest(PinItem *pin1, PinItem *pin2) {
        addWire({pin1, pin2});
    }

    void onComponentRemoveRequest(ComponentItem *component) {
        removeComponent(component);
    }

    void onDrillDownRequest(QGraphicsScene *scene) {
        main_view_->setScene(scene);
        input_mapper_->setScene(scene);
    }
    void onDrillUpRequest(QGraphicsScene *scene) {
        main_view_->setScene(scene);
        input_mapper_->setScene(scene);
    }

public:
    Controller() : main_view_(new MainView(new QGraphicsScene())), input_mapper_(new InputMapper(main_view_->scene(), this)) {
        initConnects();
    }

    MainView* mainView() {
        return main_view_;
    }

    void addWire(const std::vector<PinItem *> &pin_items) {
        // pins must have no wire
        for (const auto *pin_item : pin_items)
            if (pin_item->wire()) {
                qDebug() << "pin already has a wire";
                return;
            }

        const auto logic_pins = black_box_components_.at(pin_items[0]->owner())->pins();

        // create ui, logic wire
        auto *wire_item = new WireItem{};
        auto *logic_wire = new LogicWire{wire_item};

        // set connections in logic-side
        for (auto *logic_pin : logic_pins) {
            // set wire for pin (NOTE: we do it because pin need call wire if he was edited)
            logic_pin->setWire(logic_wire);

            // set pin for wire (NOTE: we do it because wire need change pin signal)
            logic_wire->addPin(logic_pin);
        }

        // set connections in ui-side
        for (auto *pin_item : pin_items) {
            // set wire for pin (NOTE: we do it because we need wire information at remove function)
            pin_item->setWire(wire_item);

            // set pin for wire (NOTE: we do it because wire builds its lines between pins)
            wire_item->addPin(pin_item);
        }

        // add wire_item to scene
        main_view_->scene()->addItem(wire_item);
        qDebug() << "add wire to scene";

        // add wire recording (NOTE: we do it because we need it in removeWire)
        wires_.insert({wire_item, logic_wire});
    }
    void removeWire(WireItem *wire_item) {
        // get logic wire
        auto *logic_wire = wires_.at(wire_item);

        // remove logic pins from logic_wire & remove logic_wire from pins
        for (auto *logic_pin : logic_wire->pins()) {
            logic_wire->removePin(logic_pin);
            logic_pin->removeWire();
        }

        // remove pin items from wire_item & remove wire_item from pin_items
        for (auto *pin_item : wire_item->pins()) {
            wire_item->removePin(pin_item);
            pin_item->removeWire();
        }

        // remove recording from registry
        wires_.erase(wire_item);

        // after all we can delete logic/ui wire
        delete wire_item;
        delete logic_wire;
    }

    void addComponent(ComponentItem *component_item) {
        // get logic side from factory
        auto *logic_component = logicTypeByItem(component_item);
        if (logic_component == nullptr) {
            // can`t find that type
            qDebug() << "Undefined component_item type";
            return;
        }

        // get pin vectors
        auto logic_pins = logic_component->pins();
        auto pin_items = component_item->pins();

        // fallback if vectors have different sizes
        if (logic_pins.size() != pin_items.size()) {
            qDebug() << "vectors have different sizes";
            return;
        }

        // pin.owner_ was already set in component class.

        // create pin recordings in registry & add pins to scene (NOTE: we do it because we need it in addWire)
        for (int i = 0; i < pin_items.size(); i++) {
            auto *logic_pin = logic_pins[i];
            auto *pin_item = pin_items[i];

            pins_.insert({pin_item, logic_pin});
            main_view_->scene()->addItem(pin_item);
        }

        // add component_item to scene
        main_view_->scene()->addItem(component_item);

        // add recording in black_box_components_ (NOTE: we do this because we need it in removeComponent)
        black_box_components_.insert({component_item, logic_component});
    }

    void removeComponent(ComponentItem *component_item) {
        const auto logic_component = black_box_components_.at(component_item);

        // remove wire connections from logic(wire, pin)
        for (auto *logic_pin : logic_component->pins()) {
            const auto logic_wire = logic_pin->wire();

            if (logic_wire != nullptr) {
                // remove the wire from pin
                logic_pin->removeWire();

                // remove the pin from wire
                logic_wire->removePin(logic_pin);
            }

            delete logic_pin;
        }


        // remove pin items from wire_items
        for (auto *pin_item : component_item->pins()) {
            auto *wire_item = pin_item->wire();

            if (wire_item != nullptr) {
                // remove the wire from pin
                pin_item->removeWire();

                // remove the pin from wire
                wire_item->removePin(pin_item);

                // remove wire_item from scene if it is empty
                if (wire_item->empty())
                    removeWire(wire_item);

                // remove pin recording
                pins_.erase(pin_item);
            }

            // remove pin_item from scene
            pin_item->scene()->removeItem(pin_item);

            // after all we can delete pin_item
            delete pin_item;
        }

        // remove component_item from scene
        component_item->scene()->removeItem(component_item);

        // after all we can delete logic/ui component
        delete component_item;
        delete logic_component;

        // remove pair from map
        black_box_components_.erase(component_item);
    }

    ~Controller() {
        delete input_mapper_;
    }
};
