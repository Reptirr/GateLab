#pragma once
#include <LogicComponent.h>

// just the component that always set pin signal to true
class LogicSource : public LogicComponent {
    std::shared_ptr<LogicPin> pin_;

public:
    /**
     * Please do not use this constructor. Instead use LogicComponent::create
     */
    LogicSource() = default;

    void initPins() override {
        INIT_PINS(1)

        pin_ = pins_[0];
        pin_->setSignalByOwner(true);
    }

    void handle() override {
        pins_[0]->setSignalByOwner(true);
    }

    ~LogicSource() override {
        pin_.reset();
    }
};