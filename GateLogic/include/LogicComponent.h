#pragma once
#include <LogicPin.h>

class LogicComponent {
protected:
    std::vector<LogicPin*> pins_;

public:
    LogicComponent(std::vector<LogicPin *> pins) {
        pins_ = pins;
    }

    virtual void handle() = 0;

    virtual std::vector<LogicPin*> pins() {
        return pins_;
    }

    virtual ~LogicComponent() {}
};
