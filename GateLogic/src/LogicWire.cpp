#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>
#include <WireItem.h>

LogicWire::LogicWire(WireItem *item) {
    signal_consumer = item;
}

void LogicWire::handle() {
    bool signal{};

    for (auto *pin: pins_) {
        if (pin->getSignal()) {
            signal = true;
            break;
        }
    }

    for (auto *pin: pins_) {
        pin->setSignalByWire(signal);
    }

    if (signal_consumer != nullptr) signal_consumer->setColorBySignal(signal);

}

void LogicWire::addPin(LogicPin *pin) {
    if (std::find(pins_.begin(), pins_.end(), pin) == pins_.end()) {
        pins_.push_back(pin);
        handle();
    }
}

void LogicWire::removePin(LogicPin *removedPin) {
    auto remove = std::find(pins_.begin(), pins_.end(), removedPin);

    if (remove != pins_.end()) {
        pins_.erase(remove);
    }
}
