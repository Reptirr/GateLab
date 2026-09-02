#pragma once

#include <vector>

class LogicComponent;
class IPinFul;
class LogicWire;


class LogicPin {
    bool signal_ = false;

    // pin can has connections:
    // wire - pin - transistor

    LogicComponent* owner_{}; // for component
    LogicWire* conn_{}; // only for wire

public:
    LogicPin(LogicComponent *);
    ~LogicPin();


    bool getSignal() const;

    void setSignalByWire(bool);
    void setSignalByOwner(bool);

    void setWire(LogicWire*);
    void removeWire();
};
