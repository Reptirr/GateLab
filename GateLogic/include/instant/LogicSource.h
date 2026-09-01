#pragma once
#include <LogicComponent.h>

// just the component that always set pin signal to true
class LogicSource : public LogicComponent {
    LogicPin *pin_{};

public:
    LogicSource() {
        pin_ = new LogicPin(this);
        pin_->setSignal(true);
    }

    void handle() override {
        pin_->setSignal(true);
    };

    std::vector<LogicPin *> pins() override ;
};