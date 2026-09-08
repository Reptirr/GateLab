#pragma once

#include <memory>

class LogicComponent;
class IPinFul;
class LogicWire;


class LogicPin : public std::enable_shared_from_this<LogicPin> {
    bool signal_ = false; // signal from wire (NOTE: checks by owner and sets by wire)
    bool own_signal_ = false; // signal from owner(NOTE: checks by wire and sets by owner)

    // pin can has connection:
    // wire - pin - transistor

    std::weak_ptr<LogicComponent> owner_; // only for component
    std::shared_ptr<LogicWire> conn_; // only for wire; deletes itself

public:
    explicit LogicPin(const std::weak_ptr<LogicComponent> &owner);

    bool getSignal() const;

    bool ownSignal() const;

    void setSignalByWire(bool);

    void setSignalByOwner(bool);

    void setWire(const std::shared_ptr<LogicWire> &external);

    void removeWire();

    ~LogicPin();
};
