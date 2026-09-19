#pragma once
#include <ComponentItem.h>

enum class EditMode {
    WIRE_CREATING,
    COMPONENT_EDIT
};

Q_DECLARE_METATYPE(EditMode)

struct WireCreating  {
    // for logic create
    WireItem *wire_item;

    // for moving while create
    WireNode *current_node;

    void reset() {
        wire_item = nullptr;
        current_node = nullptr;
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
    explicit Mode(WireCreating wire_creating) : Base(wire_creating) {}
    explicit Mode(ComponentEdit component_edit) : Base(component_edit) {}

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
