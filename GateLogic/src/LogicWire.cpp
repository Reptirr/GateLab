#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>
#include <WireItem.h>


LogicWire::LogicWire() {

}

void LogicWire::setSignalConsumer(WireItem *signal_consumer) {
    signal_consumer_ = signal_consumer;
}

void LogicWire::handle() {
    // delete died pins
    std::erase_if(
        pins_,
        [](const std::weak_ptr<LogicPin>& wp) {
            return wp.expired();
        }
    );



    bool signal{};

    if (pins_.size() != 0) {
        for (auto pin__ : pins_) {
            if (const auto pin = pin__.lock()) {
                if (bool own = pin->ownSignal()) {
                    signal = true;
                    break;
                }
            }
        }

        for (auto pin__ : pins_) {
            if (const auto pin = pin__.lock()) {
                pin->setSignalByWire(signal);
            }
        }
    }

    if (signal_consumer_)
        signal_consumer_->setColorBySignal(signal);
}

std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin> > LogicWire::uniteWire(std::shared_ptr<LogicWire> &logic_wire) {
    std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin> > new_pins;

    for (auto new_pin_ : logic_wire->pins_) {
        if (const auto new_pin = new_pin_.lock()) {
            new_pin->setWire(shared_from_this());
            new_pins.insert(new_pin_);
            pins_.insert(new_pin);
        }
    }

    logic_wire->pins_.clear();

    handle();

    return new_pins;
}

void LogicWire::addPin(const std::weak_ptr<LogicPin> &pin) {
    pins_.insert(pin);
    handle();
}

void LogicWire::removePin(const std::weak_ptr<LogicPin> &pin) {
    pins_.erase(pin);
    handle();
}
