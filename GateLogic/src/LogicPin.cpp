#include <LogicComponent.h>
#include <LogicPin.h>
#include <LogicWire.h>

void LogicPin::setWire(LogicWire *external) {
    conn_ = external;
}

void LogicPin::removeWire() {
    conn_ = nullptr;
}

LogicPin::LogicPin(LogicComponent * owner) {
    owner_ = owner;
}

bool LogicPin::getSignal() {
    return signal_;
}

void LogicPin::setSignalByWire(bool signal) {
    if (signal_ == signal) return;

    signal_ = signal;

    owner_->handle();
}

void LogicPin::setSignalByOwner(bool signal) {
    if (signal_ == signal) return;

    signal_ = signal;

    if (conn_ != nullptr) conn_->handle();
}
