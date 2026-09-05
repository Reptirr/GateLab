#include <LogicComponent.h>
#include <LogicPin.h>
#include <LogicWire.h>
#include <cstdio>
#include <cstdint>

#ifndef LOGICPIN_DEBUG
#define LOGICPIN_DEBUG 1
#endif

#if LOGICPIN_DEBUG
    #define PIN_LOG(...) do { fprintf(stderr, __VA_ARGS__); fflush(stderr); } while(0)
#else
    #define PIN_LOG(...) do {} while(0)
#endif

static inline unsigned shortPtr(const void *p) {
    return static_cast<unsigned>(reinterpret_cast<std::uintptr_t>(p) & 0xFFFFFu);
}
#define SP(p) shortPtr(p)


void LogicPin::setWire(LogicWire *external) {
    conn_ = external;
}

void LogicPin::removeWire() {
    conn_ = nullptr;
    signal_ = false;

    if (owner_ != nullptr) {
        owner_->handle();
    }
}

LogicPin::LogicPin(LogicComponent * owner) {
    owner_ = owner;
    conn_ = nullptr;

    PIN_LOG("[PIN %05x] constructed: owner_=%05x\n", SP(this), SP(owner_));
}

bool LogicPin::getSignal() const {
    return signal_;
}

bool LogicPin::ownSignal() const {
    return own_signal_;
}

void LogicPin::setSignalByWire(const bool s) {
    if (signal_ == s) {
        return;
    }

    signal_ = s;
    PIN_LOG("[PIN %05x] signal_ -> %d (by wire)\n", SP(this), s);

    if (owner_ != nullptr) {
        owner_->handle();
    }
}

void LogicPin::setSignalByOwner(const bool s) {
    if (own_signal_ == s) {
        return;
    }

    own_signal_ = s;
    PIN_LOG("[PIN %05x] own_signal_ -> %d\n", SP(this), s);

    if (conn_ != nullptr) {
        conn_->handle();
    } else {
        signal_ = s;
    }
}