#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>
#include <WireItem.h>
#include <cstdio>
#include <cstdint>

#ifndef LOGICPIN_DEBUG
#define LOGICPIN_DEBUG 1
#endif

#if LOGICPIN_DEBUG
    #define WIRE_LOG(...) do { fprintf(stderr, __VA_ARGS__); fflush(stderr); } while(0)
#else
    #define WIRE_LOG(...) do {} while(0)
#endif

static inline unsigned shortPtr(const void *p) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p) & 0xFFFFFu);
}
#define SP(p) shortPtr(p)


LogicWire::LogicWire(WireItem *item) {
    signal_consumer = item;
    WIRE_LOG("[WIRE %05x] constructed: signal_consumer=%05x\n", SP(this), SP(signal_consumer));
}

void LogicWire::handle() const {
    bool signal{};

    for (const auto *pin : pins_) {
        if (pin->ownSignal()) {
            signal = true;
            break;
        }
    }

    WIRE_LOG("[WIRE %05x] handle(): signal=%d, pins=%zu\n", SP(this), signal, pins_.size());

    for (auto *pin: pins_) {
        pin->setSignalByWire(signal);
    }

    if (signal_consumer != nullptr) {
        signal_consumer->setColorBySignal(signal);
    }
}

void LogicWire::addPin(LogicPin *pin) {
    if (std::find(pins_.begin(), pins_.end(), pin) == pins_.end()) {
        pins_.push_back(pin);
        WIRE_LOG("[WIRE %05x] addPin(%05x): pins=%zu\n", SP(this), SP(pin), pins_.size());
        handle();
    }
}

void LogicWire::removePin(LogicPin *removedPin) {
    auto remove = std::find(pins_.begin(), pins_.end(), removedPin);

    if (remove != pins_.end()) {
        pins_.erase(remove);
        WIRE_LOG("[WIRE %05x] removePin(%05x): pins=%zu\n", SP(this), SP(removedPin), pins_.size());
    }

    handle();
}