#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>
#include <WireItem.h>

LogicWire::LogicWire(WireItem *item) {
    signal_consumer = item;
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

    if (signal_consumer != nullptr) {
        signal_consumer->setColorBySignal(signal);
    }
}

void LogicWire::addPin(const std::weak_ptr<LogicPin> &pin) {
    pins_.insert(pin);
    handle();
}

void LogicWire::removePin(const std::weak_ptr<LogicPin> &pin) {
    pins_.erase(pin);
    handle();
}
