#pragma once
#include <ComponentItem.h>

struct WireCreating {
    PinItem *selected_pin{};
    QGraphicsLineItem *line_item{};

    void reset() {
        selected_pin = nullptr;
        if (line_item) delete line_item;
        line_item = nullptr;
    }

    ~WireCreating() {
        reset();
    }
};

struct ComponentManipulating {

};

using Mode = std::variant<
    ComponentManipulating,
    WireCreating
>;
