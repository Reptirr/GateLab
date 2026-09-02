#pragma once

#include <vector>

class WireItem;
class LogicPin;

class LogicWire {
    std::vector<LogicPin*> pins_;

    WireItem *signal_consumer{};

public:
    LogicWire() = default;
    LogicWire(WireItem *);

    void handle();

    void addPin(LogicPin*);

    void removePin(LogicPin *removedPin);
};
