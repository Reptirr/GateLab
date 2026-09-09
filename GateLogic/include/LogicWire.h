#pragma once

#include <memory>
#include <set>
#include <Utils.h>
#include <vector>

class WireItem;
class LogicPin;

class LogicWire {
    std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin>> pins_;

    WireItem *signal_consumer{};

public:
    LogicWire() = default;
    explicit LogicWire(WireItem *);

    void handle();

    void addPin(const std::weak_ptr<LogicPin> &pin);
    void removePin(const std::weak_ptr<LogicPin> &pin);
};
