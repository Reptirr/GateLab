#pragma once

#include <vector>

class LogicComponent;
class IPinFul;
class LogicWire;


class LogicPin {
    bool signal_ = false;

    // pin can has connections:
    // wire - pin - wire
    // wire - pin - transistor

    LogicWire* conn_{}; // only for wire
    LogicComponent* owner_{}; // for transistor or wire

public:
    LogicPin(LogicComponent *);

    bool getSignal();

    void setSignalByWire(bool);
    void setSignalByOwner(bool);

    void setWire(LogicWire*);
    void removeWire();
};
