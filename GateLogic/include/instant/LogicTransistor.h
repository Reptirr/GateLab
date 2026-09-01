#pragma once

#include <LogicComponent.h>
#include <LogicPin.h>

class LogicPin;


class LogicTransistor : public LogicComponent {
    LogicPin* left_pin_{};
    LogicPin* top_pin_{};
    LogicPin* right_pin_{};

public:
    LogicTransistor() {
        left_pin_ = new LogicPin(this);
        top_pin_ = new LogicPin(this);
        right_pin_ = new LogicPin(this);
    }

    std::vector<LogicPin *> pins() override {
        return { left_pin_, top_pin_, right_pin_ };
    };

    std::tuple<LogicPin*, LogicPin*, LogicPin*> pinsTuple() {
        return { left_pin_, top_pin_, right_pin_ };
    }

    void handle() override {
        // NOTE: по сути транзистор = AND по механике сигналов, следовательно можно исплоьзовать && вместо условий
        right_pin_->setSignalByOwner( top_pin_->getSignal() && left_pin_->getSignal() );
    }
};