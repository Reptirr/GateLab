#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>
#include <WireItem.h>
#include <cstdio>

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
        if (auto pin = pin__.lock()) {
            bool own = pin->ownSignal();
            if (own) {
                signal = true;
                break;
            }
        }
    }

    for (auto pin__ : pins_) {
        if (auto pin = pin__.lock()) {
            pin->setSignalByWire(signal);
        }
    }

    if (signal_consumer != nullptr) {
        signal_consumer->setColorBySignal(signal);
    }
}

void LogicWire::addPin(std::weak_ptr<LogicPin> pin) {
    pins_.push_back(pin);
    handle();
}
