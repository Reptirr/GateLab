#pragma once

#include <LogicComponent.h>
#include <LogicPin.h>

class LogicPin;


class LogicTransistor : public LogicComponent {
    LogicPin* left_pin_{};
    LogicPin* top_pin_{};
    LogicPin* right_pin_{};

public:
    LogicTransistor() :
    LogicComponent({new LogicPin(this), new LogicPin(this), new LogicPin(this)}),
    left_pin_(pins_[0]),
    top_pin_(pins_[1]),
    right_pin_(pins_[2])
    {

    }

    std::tuple<LogicPin*, LogicPin*, LogicPin*> pinsTuple() {
        return { left_pin_, top_pin_, right_pin_ };
    }

    void handle() override {
        qDebug() << "top_pin_: " << static_cast<void*>(top_pin_);

        // NOTE: по сути транзистор = AND по механике сигналов, следовательно можно исплоьзовать && вместо условий
        right_pin_->setSignalByOwner(
            top_pin_->getSignal() &&
            left_pin_->getSignal()
        );
    }
};