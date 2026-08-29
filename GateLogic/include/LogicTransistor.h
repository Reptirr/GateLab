#pragma once

#include <IPinFul.h>
#include <LogicPin.h>

class LogicPin;


class LogicTransistor : public IPinFul {
    LogicPin* left_pin_;
    LogicPin* top_pin_;
    LogicPin* right_pin_;
public:
    LogicTransistor(LogicPin* left_pin, LogicPin* top_pin, LogicPin* right_pin) : left_pin_(left_pin),
        top_pin_(top_pin), right_pin_(right_pin) {
    }


    void handle() override {
        // NOTE: по сути транзистор = AND по механике сигналов, следовательно можно исплоьзовать && вместо условий
        right_pin_->setSignal( top_pin_->getSignal() && left_pin_->getSignal() );
    }
};