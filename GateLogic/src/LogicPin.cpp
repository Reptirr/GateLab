#include <LogicComponent.h>
#include <LogicPin.h>
#include <LogicWire.h>
#include <cstdio>

// ---- Отладочный макрос, чтобы можно было легко выключить весь дебаг разом ----
#ifndef LOGICPIN_DEBUG
#define LOGICPIN_DEBUG 1
#endif

#if LOGICPIN_DEBUG
    #define PIN_LOG(...) do { fprintf(stderr, __VA_ARGS__); fflush(stderr); } while(0)
#else
    #define PIN_LOG(...) do {} while(0)
#endif


void LogicPin::setWire(LogicWire *external) {
    PIN_LOG("[PIN %p] setWire: conn_ %p -> %p\n", (void*)this, (void*)conn_, (void*)external);

    conn_ = external;

    PIN_LOG("[PIN %p] setWire: calling conn_->handle() on wire %p\n", (void*)this, (void*)conn_);
    conn_->handle();
}

void LogicPin::removeWire() {
    PIN_LOG("[PIN %p] removeWire: conn_ %p -> nullptr\n", (void*)this, (void*)conn_);

    conn_ = nullptr;
    signal_ = false;

    if (owner_ != nullptr) {
        PIN_LOG("[PIN %p] removeWire: calling owner_->handle() on owner %p\n", (void*)this, (void*)owner_);
        owner_->handle();
    } else {
        PIN_LOG("[PIN %p] removeWire: owner_ is nullptr, skip handle()\n", (void*)this);
    }
}

LogicPin::LogicPin(LogicComponent * owner) {
    owner_ = owner;
    conn_ = nullptr;

    PIN_LOG("[PIN %p] constructed: owner_=%p conn_=nullptr signal_=%d own_signal_=%d\n",
            (void*)this, (void*)owner_, signal_, own_signal_);
}

bool LogicPin::getSignal() const {
    PIN_LOG("[PIN %p] getSignal() -> %d\n", (void*)this, signal_);
    return signal_;
}

bool LogicPin::ownSignal() const {
    PIN_LOG("[PIN %p] ownSignal() -> %d\n", (void*)this, own_signal_);
    return own_signal_;
}

void LogicPin::setSignalByWire(const bool s) {
    PIN_LOG("[PIN %p] setSignalByWire(%d) called: current signal_=%d\n", (void*)this, s, signal_);

    if (signal_ == s) {
        PIN_LOG("[PIN %p] setSignalByWire(%d): no change, skip\n", (void*)this, s);
        return;
    }

    signal_ = s;
    PIN_LOG("[PIN %p] setSignalByWire(%d): signal_ updated -> %d\n", (void*)this, s, signal_);

    if (owner_ != nullptr) {
        PIN_LOG("[PIN %p] setSignalByWire(%d): calling owner_->handle() on owner %p\n", (void*)this, s, (void*)owner_);
        owner_->handle();
    } else {
        PIN_LOG("[PIN %p] setSignalByWire(%d): owner_ is nullptr, skip handle()\n", (void*)this, s);
    }
}

void LogicPin::setSignalByOwner(const bool s) {
    PIN_LOG("[PIN %p] setSignalByOwner(%d) called: current own_signal_=%d, owner_=%p, conn_=%p\n",
            (void*)this, s, own_signal_, (void*)owner_, (void*)conn_);

    if (own_signal_ == s) {
        PIN_LOG("[PIN %p] setSignalByOwner(%d): no change, skip\n", (void*)this, s);
        return;
    }

    own_signal_ = s;
    PIN_LOG("[PIN %p] setSignalByOwner(%d): own_signal_ updated -> %d\n", (void*)this, s, own_signal_);

    if (conn_ != nullptr) {
        PIN_LOG("[PIN %p] setSignalByOwner(%d): conn_ != nullptr, calling conn_->handle() on wire %p\n",
                (void*)this, s, (void*)conn_);
        conn_->handle();
    } else {
        PIN_LOG("[PIN %p] setSignalByOwner(%d): conn_ is nullptr, directly setting signal_ -> %d\n",
                (void*)this, s, s);
        signal_ = s;
    }
}