#pragma once

#include <LogicUtils.h>
#include <memory>
#include <qcolor.h>
#include <set>
#include <unordered_set>

class QColor;
class WireItem;
class LogicPin;

class LogicWire : public std::enable_shared_from_this<LogicWire>{
    std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin>> pins_;

    WireItem *signal_consumer_;

public:
    LogicWire();
    void setSignalConsumer(WireItem *);

    void handle();

    std::set<std::weak_ptr<LogicPin>, WeakPtrComparator<LogicPin>> uniteWire(std::shared_ptr<LogicWire> &logic_wire);

    void addPin(const std::weak_ptr<LogicPin> &pin);
    void removePin(const std::weak_ptr<LogicPin> &pin);
};
