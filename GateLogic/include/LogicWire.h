#pragma once

#include <IPinFul.h>
#include <vector>

class LogicPin;

class LogicWire : public IPinFul {
    std::vector<LogicPin*> pins_;

public:
    void handle() override;

    void addPin(LogicPin*);
};
