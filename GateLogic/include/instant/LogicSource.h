#pragma once
#include <LogicComponent.h>

// just the component that always set pin signal to true
class LogicSource : public LogicComponent {
    LogicPin *pin_;

public:
    LogicSource() :
    LogicComponent({new LogicPin(this)}),
    pin_(pins_[0])
    {
        pin_->setSignalByOwner(true);
    }

    void handle() override {
        pin_->setSignalByOwner(true);
    }
};