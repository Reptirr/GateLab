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

    std::unordered_map<PinItem*, std::weak_ptr<LogicPin>> pins_;
    std::unordered_map<WireItem*, std::weak_ptr<LogicWire>> wires_;
    std::unordered_map<ComponentItem*, std::shared_ptr<LogicComponent>> black_box_components_; // for component removing

    std::set<ComponentItem*> drillable_components_;

    void initConnects() {
        // MainView -> InputMapper
        connect(main_view_, &MainView::keyPress,
                input_mapper_, &InputMapper::onKeyPress);
        connect(main_view_, &MainView::mouseDoubleClick,
                input_mapper_, &InputMapper::onMouseDoubleClick);
        connect(main_view_, &MainView::mouseMove,
                input_mapper_, &InputMapper::onMouseMove);
        connect(main_view_, &MainView::mousePress,
                input_mapper_, &InputMapper::onMousePress);

        // InputMapper -> Controller
        connect(input_mapper_, &InputMapper::drillDownRequest, // drill-down
                this, &Controller::onDrillDownRequest);
        connect(input_mapper_, &InputMapper::drillUpRequest, // drill-up
                this, &Controller::onDrillUpRequest);
        connect(input_mapper_, &InputMapper::wireCreateRequest, // wire-create
                this, &Controller::onWireCreateRequest);
        connect(input_mapper_, &InputMapper::addPinToWireRequest, // pin-add
                this, &Controller::onAddPinToWireRequest);
        connect(input_mapper_, &InputMapper::removePinFromWireRequest, //pin-remove
                this, &Controller::onRemovePinFromWireRequest);
        connect(input_mapper_, &InputMapper::componentRemoveRequest, // component-delete
                this, &Controller::onComponentRemoveRequest);
        connect(input_mapper_, &InputMapper::componentCreateRequest,
                this, &Controller::onComponentCreateRequest); // component-create
    }

public slots:
    void onComponentCreateRequest(ComponentItem *component_item) {
        addComponent(component_item);
    }
    void onComponentRemoveRequest(ComponentItem *component) {
        removeComponent(component);
    }

    void onWireCreateRequest(WireEndPoint *point1, WireEndPoint *point2) {
        addWire({point1, point2});
    }
    void onAddPinToWireRequest(WireItem *wire_item, PinItem *pin_item) const {
        addPinToWire(wire_item, pin_item);
    }
    void onRemovePinFromWireRequest(WireItem *wire_item, PinItem *pin_item) {
        removePinFromWire(wire_item, pin_item);
    }


    void onDrillDownRequest(QGraphicsScene *scene) const {
        main_view_->setScene(scene);
        input_mapper_->setScene(scene);
    }
    void onDrillUpRequest(QGraphicsScene *scene) const {
        main_view_->setScene(scene);
        input_mapper_->setScene(scene);
    }

public:
    Controller() : main_view_(new MainView(new QGraphicsScene())), input_mapper_(new InputMapper(main_view_->scene(), this)) {
        initConnects();
    }

    MainView *mainView() const {
        return main_view_;
    }

    InputMapper *inputMapper() const {
        return input_mapper_;
    }

    void addWire(const std::vector<WireEndPoint *> &point_items) {
        // need 2 pins to connect
        my_assert(point_items.size() >= 2);

        // cant connect already connected pins
        for (const auto point : point_items) {
            if (point->lines().size()) {
                qDebug() << "pin already has a wire";
                return;
            }
        }

        // cant connect the same pins
        if (const std::unordered_set<WireEndPoint*> temp_map{point_items.begin(),point_items.end()};  point_items.size() != temp_map.size()) {
            qDebug() << "cant connect the same pins";
            return;
        }

        // LOGIC: get logic pins and create a wire and set pins for wire & set wire for pins
        // UI: set 2 pins in WireItem constructor, other in cycle
        // no recordings

        auto *wire_item = new WireItem{point_items[0], point_items[1]};
        main_view_->scene()->addItem(wire_item);

        std::vector<std::weak_ptr<LogicPin>> logic_pins{};
        std::vector<PinItem *> pin_items{};
        for (auto *point_item : point_items) {
            if (auto *pin_item = dynamic_cast<PinItem *>(point_item)) {
                logic_pins.push_back(pins_[pin_item]);
                pin_items.push_back(pin_item);
            }
        }

        // temp hook for addPinToWire
        auto logic_wire = std::make_shared<LogicWire>(wire_item);

        // add recording
        wires_.insert({wire_item, logic_wire});

        // set pins for wire & wire for pins at logic-side
        for (auto *pin_item : pin_items) {
            addPinToWire(wire_item, pin_item);
        }

        // add item to scene
        main_view_->scene()->addItem(wire_item);
    }

    void addPinToWire(WireItem *wire_item, PinItem* pin_item) const {
        const auto logic_pin = pins_.at(pin_item);
        const auto logic_wire = wires_.at(wire_item);

        const auto sh_pin = logic_pin.lock();
        const auto sh_wire = logic_wire.lock();

        if (sh_pin && sh_wire) {
            sh_pin->setWire(sh_wire);
            sh_wire->addPin(sh_pin);
        }
    }

    void removePinFromWire(WireItem *wire_item, PinItem* pin_item) {
        const auto sh_pin =  pins_.at(pin_item).lock();
        const auto sh_wire = wires_.at(wire_item).lock();

        if (sh_wire && sh_pin) {
            sh_pin->removeWire();
            sh_wire->removePin(sh_pin);
        }

        wire_item->removeEndPoint(pin_item);
    }

    void addComponent(ComponentItem *component_item) {
        // LOGIC: pins construction is in the component
        // UI: the same, also add to scene
        // create recording of component and his pins

        // get logic component from factory (item -> logic)
        auto logic_component = std::move(logicTypeByItem(component_item));
        if (logic_component == nullptr) return; // undefined item

        main_view_->scene()->addItem(component_item);

        // add recordings & add pins to scene
        for (int i = 0; i < component_item->pins().size(); i++) {
            auto *pin_item = component_item->pins()[i];
            auto logic_pin = logic_component->pins()[i];

            main_view_->scene()->addItem(pin_item);
            pins_[pin_item] = logic_pin;
        }

        // create recording in bb_components
        black_box_components_[component_item] = std::move(logic_component);
    }

    void removeComponent(ComponentItem *component_item) {
        // LOGIC: shared ptr of logic component has only 1 hook in controller.
        // after hook reset component will call pins destructors
        // and them will reset theirs shared ptrs of wires (NOTE: wire delete died ptrs on pins before each handle)
        // UI: just delete component_item. after that it will call destructors of pins and them will remove pin from their wires
        // also remove recordings of component and pins

        // remove item_pins
        for (auto *pin : component_item->pins()) {
            if (pin->parentWire())
                removePinFromWire(pin->parentWire(), pin);

            pins_.erase(pin);
        }

        // reset hook on logic_component
        black_box_components_.extract(component_item).mapped().reset();

        delete component_item;
    }

    ~Controller() override {
        delete input_mapper_;
    }
};
