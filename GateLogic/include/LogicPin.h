#pragma once

#include <vector>

class IPinFul;
class LogicWire;


class LogicPin {
    bool signal_ = false;

    std::vector<IPinFul*> handlers_;

public:
    bool getSignal();

    void setSignal(bool);

    void addHandler(IPinFul *handle);
};
