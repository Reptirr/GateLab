#pragma once
#include <vector>

class LogicPin;

class IPinFul {
protected:
    std::vector<LogicPin*> pins_;

public:
    virtual void handle() = 0;

    virtual ~IPinFul() = default;
};
