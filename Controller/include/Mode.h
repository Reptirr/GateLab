#pragma once
#include <ComponentItem.h>

enum class EditMode {
    WIRE_CREATING,
    COMPONENT_EDIT
};

Q_DECLARE_METATYPE(EditMode)

struct WireCreating  {
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

struct ComponentEdit  {

};


class Mode : public std::variant<ComponentEdit, WireCreating> {
    using Base = std::variant<ComponentEdit, WireCreating>;

public:
    Mode() : Base(ComponentEdit{}) {}
    Mode(WireCreating wire_creating) : Base(wire_creating) {}
    Mode(ComponentEdit component_edit) : Base(component_edit) {}

    void emplaceByEnum(const EditMode edit_mode) {
        switch (edit_mode) {
            case EditMode::WIRE_CREATING:
                emplace<WireCreating>();
                break;

            case EditMode::COMPONENT_EDIT:
                emplace<ComponentEdit>();
                break;
        }
    }
};
