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
            pin->setSignal(true);
        }
    }
}

void LogicWire::addPin(LogicPin *pin) {
    if (std::find(pins_.begin(), pins_.end(), pin) == pins_.end()) {
        pins_.push_back(pin);
    }
}
