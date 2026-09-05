#include <algorithm>
#include <LogicWire.h>
#include <LogicPin.h>
#include <WireItem.h>
#include <cstdio>

#ifndef LOGICPIN_DEBUG
#define LOGICPIN_DEBUG 1
#endif

#if LOGICPIN_DEBUG
    #define WIRE_LOG(...) do { fprintf(stderr, __VA_ARGS__); fflush(stderr); } while(0)
#else
    #define WIRE_LOG(...) do {} while(0)
#endif


LogicWire::LogicWire(WireItem *item) {
    signal_consumer = item;
    WIRE_LOG("[WIRE %p] constructed: signal_consumer=%p\n", (void*)this, (void*)signal_consumer);
}

void LogicWire::handle() const {
    WIRE_LOG("[WIRE %p] handle(): begin, pins.size()=%zu\n", (void*)this, pins_.size());

    bool signal{};

    for (const auto *pin : pins_) {
        bool own = pin->ownSignal();
        WIRE_LOG("[WIRE %p] handle(): pin %p ownSignal()=%d\n", (void*)this, (const void*)pin, own);
        if (own) {
            signal = true;
            WIRE_LOG("[WIRE %p] handle(): found driving pin %p, result=true, stop scanning\n", (void*)this, (const void*)pin);
            break;
        }
    }

    WIRE_LOG("[WIRE %p] handle(): computed signal=%d, propagating to %zu pins\n", (void*)this, signal, pins_.size());

    for (auto *pin: pins_) {
        WIRE_LOG("[WIRE %p] handle(): calling setSignalByWire(%d) on pin %p\n", (void*)this, signal, (void*)pin);
        pin->setSignalByWire(signal);
    }

    if (signal_consumer != nullptr) {
        WIRE_LOG("[WIRE %p] handle(): updating signal_consumer %p color, signal=%d\n", (void*)this, (void*)signal_consumer, signal);
        signal_consumer->setColorBySignal(signal);
    } else {
        WIRE_LOG("[WIRE %p] handle(): signal_consumer is nullptr, skip color update\n", (void*)this);
    }

    WIRE_LOG("[WIRE %p] handle(): end\n", (void*)this);
}

void LogicWire::addPin(LogicPin *pin) {
    WIRE_LOG("[WIRE %p] addPin(%p) called\n", (void*)this, (void*)pin);

    if (std::find(pins_.begin(), pins_.end(), pin) == pins_.end()) {
        pins_.push_back(pin);
        WIRE_LOG("[WIRE %p] addPin(%p): pin added, pins.size()=%zu, calling handle()\n", (void*)this, (void*)pin, pins_.size());
        handle();
    } else {
        WIRE_LOG("[WIRE %p] addPin(%p): pin already present, skip\n", (void*)this, (void*)pin);
    }
}

void LogicWire::removePin(LogicPin *removedPin) {
    WIRE_LOG("[WIRE %p] removePin(%p) called\n", (void*)this, (void*)removedPin);

    auto remove = std::find(pins_.begin(), pins_.end(), removedPin);

    if (remove != pins_.end()) {
        pins_.erase(remove);
        WIRE_LOG("[WIRE %p] removePin(%p): pin erased, pins.size()=%zu\n", (void*)this, (void*)removedPin, pins_.size());
    } else {
        WIRE_LOG("[WIRE %p] removePin(%p): pin not found in pins_\n", (void*)this, (void*)removedPin);
    }
}