#pragma once
#include <BiMap.h>
#include <InputMapper.h>
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
        connect(input_mapper_, &InputMapper::transistorCreateRequest, // transistor-create
                this, &Controller::onTransistorCreateRequest);
        connect(input_mapper_, &InputMapper::wireCreateRequest, // wire-create
                this, &Controller::onWireCreateRequest);
        connect(input_mapper_, &InputMapper::sourceCreateRequest, // source-create
                this, &Controller::onSourceCreateRequest);
        connect(input_mapper_, &InputMapper::componentRemoveRequest,
                this, &Controller::onComponentRemoveRequest); // component-delete
    }

public slots:
    void onTransistorCreateRequest(QPointF pos) {
        auto size = TransistorItem{{0,0}}.boundingRect().size();

        pos.setX(pos.x() - size.width()/2);
        pos.setY(pos.y() - size.height()/2);

        addTransistor(new TransistorItem(pos));
    }
    void onSourceCreateRequest(QPointF pos) {
        auto size = SourceItem{{0, 0}}.boundingRect().size();

        pos.setX(pos.x() - size.width()/2);
        pos.setY(pos.y() - size.height()/2);

        addSource(new SourceItem(pos));
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

    void addWire(std::vector<PinItem *> pin_items) {
        // pins must have no wire
        for (const auto *pin_item : pin_items)
            if (pin_item->wire())
                return;

        // get logic_pins
        std::vector<LogicPin *> logic_pins{};
        for (auto *pin_item : pin_items)
            if (auto it = pins_.extract(pin_item))
                logic_pins.push_back(it.mapped());
            else
                return; // there is no recording of that pin_item

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

    }

    void addTransistor(TransistorItem* component) {
        // =========
        // UI
        // =========

        // add component to scene
        main_view_->scene()->addItem(component);

        // get pins from component
        std::vector<PinItem *> pin_items = component->pins();

        // add pins item to scene
        for (auto *pin : pin_items) {
            main_view_->scene()->addItem(pin);
        }


        // =========
        // LOGIC
        // =========

        // create transistor
        auto *transistor = new LogicTransistor();

        // get pins
        auto [left, top, right] = transistor->pinsTuple();

        // add transistor to registry
        black_box_components_.insert({component, transistor});

        // add pins to registry
        pins_.insert({pin_items[0], left});
        pins_.insert({pin_items[1], top});
        pins_.insert({pin_items[2], right});
    }

    void addSource(SourceItem* item) {
        // =========
        // UI
        // =========

        // add component to scene
        main_view_->scene()->addItem(item);

        // get pins from component
        std::vector<PinItem *> pin_items = item->pins();

        // add pins item to scene
        for (auto *pin : pin_items) {
            main_view_->scene()->addItem(pin);
        }


        // =========
        // LOGIC
        // =========

        // create transistor
        auto *source = new LogicSource();

        // get pins
        auto logic_pin = source->pins()[0];

        // add transistor to registry
        black_box_components_.insert({item, source});

        // add pins to registry
        pins_.insert({pin_items[0], logic_pin});
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
        }

        // after all we can delete logic component
        delete logic_component;

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
                    wire_item->scene()->removeItem(wire_item);
            }

            // remove pin_item from scene
            pin_item->scene()->removeItem(pin_item);

            // after all we can delete pin_item
            delete pin_item;
        }

        // remove component_item from scene
        component_item->scene()->removeItem(component_item);

        // after all we can delete component_item
        delete component_item;

        // remove pair from map
        black_box_components_.erase(component_item);
    }

    ~Controller() {
        delete input_mapper_;
    }
};
