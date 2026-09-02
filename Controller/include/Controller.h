#pragma once
#include <InputMapper.h>
#include <LogicPin.h>
#include <LogicWire.h>
#include <MainView.h>
#include <PinItem.h>
#include <../../MultiScheme/include/instant/TransistorItem.h>
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
 * Component deleting his pins; pin remove himself in wire;
 *
 * Wire does not delete anything
 *
 */

class Controller : public QObject {
    Q_OBJECT

    MainView *main_view_{};
    InputMapper *input_mapper_;

    std::unordered_map<PinItem*, LogicPin*> pins_;
    std::unordered_map<WireItem*, LogicWire*> wires_;
    std::unordered_map<ComponentItem*, LogicComponent*> black_box_components_;

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
        addWire(pin1, pin2);
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
    Controller() : main_view_(new MainView(new QGraphicsScene())), input_mapper_(new InputMapper(main_view_->scene())) {
        initConnects();
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
        auto *wire_logic = new LogicWire(wire_item);

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
        pins_.at(pin1)->setWire(wire_logic);
        pins_.at(pin2)->setWire(wire_logic);

        // add pins to wire in logic
        wire_logic->addPin(pins_.at(pin1));
        wire_logic->addPin(pins_.at(pin2));


        // add wire to registry
        wires_.insert({wire_item, wire_logic});
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

    void removeComponent(ComponentItem *component) {
        // firstly delete logic, after that delete ui because logic wire has the ptr to wire item

        // =========
        // Delete black-box component
        // =========

        // ======
        // LOGIC
        // ======

        // also extract recording of component from registry
        if (auto it = black_box_components_.extract(component); it)
            delete it.mapped(); // delete all logic side of this component(pin and recording in wire)
        else return;


        // ======
        // UI
        // ======

        component->scene()->removeItem(component);

        // remove pins from registry
        for (auto *pin : component->pins()) {
            pins_.erase(pin);
        }

        delete component;
    }

    ~Controller() {
        delete input_mapper_;
    }
};
