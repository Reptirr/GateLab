#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>

void LogicWire::handle() {
    bool signal{};

    for (auto *pin: pins_) {
        if (pin->getSignal()) {
            signal = true;
            break;
        }
    }

    if (signal) {
        for (auto *pin: pins_) {
            pin->setSignalByWire(true);
        }
    }
}

void LogicWire::addPin(LogicPin *pin) {
    if (std::find(pins_.begin(), pins_.end(), pin) == pins_.end()) {
        pins_.push_back(pin);
    }
}

void LogicWire::removePin(LogicPin *removedPin) {
    auto remove = std::find(pins_.begin(), pins_.end(), removedPin);

    if (remove != pins_.end()) {
        pins_.erase(remove);
    }
}
