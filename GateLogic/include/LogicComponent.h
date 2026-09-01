#pragma once
#include <LogicPin.h>

class LogicComponent {
public:
    virtual void handle() = 0;

    virtual std::vector<LogicPin*> pins() = 0;

    virtual ~LogicComponent() = default;
};
