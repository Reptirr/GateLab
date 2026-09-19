#pragma once

#include <LogicComponent.h>
#include <LogicPin.h>

class LogicNTransistor : public LogicComponent {
    std::shared_ptr<LogicPin> left_pin_;
    std::shared_ptr<LogicPin> top_pin_;
    std::shared_ptr<LogicPin> right_pin_;

public:
    /**
     * Please do not use this constructor. Instead use LogicComponent::create
     */
    LogicNTransistor() = default;

    void initPins() override {
        INIT_PINS(3);
        left_pin_ = pins_[0];
        top_pin_ = pins_[1];
        right_pin_ = pins_[2];
    }

    void handle() override {
        right_pin_->setSignalByOwner(
            left_pin_->getSignal() && !top_pin_->getSignal()
        );
    }

    ~LogicNTransistor() override {
        left_pin_.reset();
        top_pin_.reset();
        right_pin_.reset();
    }
};