#pragma once
#include <LogicComponent.h>
#include <LogicUtils.h>
#include <LogicWire.h>
#include <memory>
#include <my_assert.h>
#include <unordered_set>


/*
 * Can:
 *  - create components
 *  - delete components
 *
 *  - create wire
 *  - delete wire
 *  - modify wire (add, remove pin)
 *
 */

/**
 * rules logic side.
 *
 * Shared ptr of components only at LogicComponent
 */
class LogicController {
    std::unordered_set<std::shared_ptr<LogicComponent>> components_{};
    std::unordered_set<std::weak_ptr<LogicWire>, WeakPtrComparator<LogicWire>> wires_{};

public:
    /**
     * invalidate shared_ptr of component from args
     *
     * @param component__ Component which we create. Need be not expires
     */
    std::weak_ptr<LogicComponent> createComponent(const std::weak_ptr<LogicComponent>& component__) {
        auto component = component__.lock();

        my_assert(!component__.expired());
        my_assert(component->pins().size() > 0);

        components_.insert(component);

        return component;
    }

    /**
     * Delete component & disconnect all pins from wires. wires may be invalid after that
     *
     * @param component__ component which we delete. will invalids & need be not expired
     */
    void removeComponent(const std::weak_ptr<LogicComponent>& component__) {
        auto component = component__.lock();

        my_assert(!component__.expired());

        my_assert(components_.contains(component));

        components_.extract(component).value().reset(); // after that
    }

    /**
     * Create wire between pins
     *
     * @param pins__ need be without connections & at least 1 pin
     * @param signal_consumer item who we will say our signal
     */
    std::weak_ptr<LogicWire> createWire(const std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin>> &pins__, WireItem *signal_consumer = nullptr) const {
        std::unordered_set<std::shared_ptr<LogicPin>> pins{};

        // check & create pins set
        for (auto pin__ : pins__) {
            my_assert(!pin__.expired()); // pin exists
            my_assert(pin__.lock()->wire().expired()); // pin without connects
            my_assert(components_.contains(pin__.lock()->owner().lock())); // we have owners of these pins

            pins.insert(pin__.lock());
        }

        const auto wire = std::make_shared<LogicWire>();
        if (signal_consumer)
            wire->setSignalConsumer(signal_consumer);

        // connect
        for (auto pin : pins) {
            wire->addPin(pin);
            pin->setWire(wire); // no reason to use this::connectPin (it`s just alias)
        }

        // handle wire
        wire->handle();

        return wire;
    }

    /**
     * remove wire from pins that in wire.
     *
     * @param wire__ wire which we remove
     */
    void removeWire(std::weak_ptr<LogicWire> wire__) {
        auto wire = wire__.lock();

        my_assert(!wire__.expired()); // wire exists

        std::unordered_set<std::shared_ptr<LogicPin>> pins{};

        // check & create pins
        for (auto pin__ : wire->pins()) {
            my_assert(!pin__.expired()); // pin exists
            my_assert(components_.contains(pin__.lock()->owner().lock())); // we have that component

            pins.insert(pin__.lock());
        }

        // remove wire from pins. after that wire is invalid
        for (auto pin : pins) {
            pin->removeWire();
        }
    }

    /**
     * create connection in pin. safe alias for pin::setWire
     *
     * @param wire__ connect to
     * @param pin__ connect what
     */
    void connectPin(const std::weak_ptr<LogicWire> &wire__, const std::weak_ptr<LogicPin> &pin__) const {
        my_assert(!wire__.expired()); // wire exists
        my_assert(!pin__.expired()); // pin exists
        my_assert(pin__.lock()->wire().expired()); // pin disconnected

        pin__.lock()->setWire(wire__.lock());
        wire__.lock()->addPin(pin__);

        wire__.lock()->handle();
    }

    /**
     * remove connection from pin. wire may be invalid after that. safe alias for pin::removeWire
     *
     * @param wire__ disconnect from
     * @param pin__ disconnect what
     */
    void disconnectPin(const std::weak_ptr<LogicWire> &wire__, const std::weak_ptr<LogicPin> &pin__) {
        my_assert(!wire__.expired()); // wire exists
        my_assert(!pin__.expired()); // pin exists
        my_assert(pin__.lock()->wire().lock().get() == wire__.lock().get()); // pin connected

        pin__.lock()->removeWire(); // after that wire may be invalid

        if (auto wire = wire__.lock())
            wire->handle();
    }


    void uniteWire(std::weak_ptr<LogicWire> first, std::weak_ptr<LogicWire> second) {
        my_assert(!first.expired());
        my_assert(!second.expired());

        auto sh_second = second.lock();

        first.lock()->uniteWire(sh_second);
    }
};
