#pragma once

#include <vector>

class LogicPin;

class LogicWire {
    std::vector<LogicPin*> pins_;

public:
    void handle();

    void addPin(LogicPin*);

    void removePin(LogicPin *removedPin);
};
