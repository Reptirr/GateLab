#include <algorithm>
#include <LogicPin.h>
#include <LogicWire.h>


void LogicPin::setSignal(bool new_signal) {
    if (signal_ == new_signal) return;

    signal_ = new_signal;

    for (auto *conn : handlers_) {
        conn->handle();
    }
}

void LogicPin::addHandler(IPinFul* handle) {
    if (std::find(handlers_.begin(), handlers_.end(), handle) == handlers_.end()) {
        handlers_.push_back(handle);
    }
}

bool LogicPin::getSignal() {
    return signal_;
}
