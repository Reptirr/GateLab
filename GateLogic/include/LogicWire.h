#pragma once

#include <memory>
#include <vector>

class WireItem;
class LogicPin;

class LogicWire {
    std::vector<std::weak_ptr<LogicPin>> pins_;

    WireItem *signal_consumer{};

public:
    LogicWire() = default;
    explicit LogicWire(WireItem *);

    void handle();

    void addPin(std::weak_ptr<LogicPin> pin);
};
