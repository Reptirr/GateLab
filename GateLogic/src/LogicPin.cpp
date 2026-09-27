#include <LogicComponent.h>
#include <LogicPin.h>
#include <LogicWire.h>

void LogicPin::setWire(std::shared_ptr<LogicWire> external) {
    conn_ = std::move(external);
}

void LogicPin::removeWire() {
    if (conn_ == nullptr) return; // already no conn

    conn_->removePin(weak_from_this());
    conn_.reset();
    signal_ = false; // there is no source of signal anymore

    if (auto ptr = owner_.lock()) {
        ptr->handle();
    }
}

std::weak_ptr<LogicWire> LogicPin::wire() {
    return conn_;
}

std::weak_ptr<LogicComponent> LogicPin::owner() {
    return owner_;
}

LogicPin::LogicPin(const std::weak_ptr<LogicComponent> &owner) {
    owner_ = owner;
}

bool LogicPin::signal() const {
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

    if (auto owner = owner_.lock()) {
        owner->handle();
    }
}

void LogicPin::setSignalByOwner(const bool s) {

    if (own_signal_ == s) {
        return;
    }

    own_signal_ = s;

    if (conn_ != nullptr) {
        conn_->handle();
    } else {
        signal_ = s;
    }
}

LogicPin::~LogicPin() {
    if (conn_) conn_->removePin(weak_from_this());
}
