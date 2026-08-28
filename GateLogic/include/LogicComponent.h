#pragma once
#include <IPinFul.h>
#include <vector>

class LogicPin;

// logic component with black-box logic
class LogicComponent : public IPinFul {

public:
    explicit LogicComponent(const std::vector<LogicPin*> &pins) {
        pins_ = pins;
    }

};