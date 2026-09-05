#pragma once

#include <vector>

class LogicComponent;
class IPinFul;
class LogicWire;


class LogicPin {
    bool signal_ = false; // signal from wire (NOTE: checks by owner and sets by wire)
    bool own_signal_ = false; // signal from owner(NOTE: checks by wire and sets by owner)

    // pin can has connections:
    // wire - pin - transistor

    LogicComponent* owner_{}; // only for component
    LogicWire* conn_{}; // only for wire

public:
    LogicPin(LogicComponent *);

    bool getSignal() const;

    bool ownSignal() const;

    void setSignalByWire(bool);
    void setSignalByOwner(bool);

    LogicComponent *owner() const {
        return owner_;
    }
    // returns nullptr if there is no wire
    LogicWire *wire() const {
        return conn_;
    }

    void setWire(LogicWire*);
    void removeWire();
};
