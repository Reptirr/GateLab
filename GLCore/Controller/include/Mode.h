#pragma once
#include <ComponentItem.h>
#include <stack>

enum class ModeType {
    COMPONENTS,
    WIRES
};

Q_DECLARE_METATYPE(ModeType)

enum class ComponentType {
    Transistor,
    NTransistor,
    Source
};

struct Mode {
    virtual ~Mode() {}
};

struct WireCreating : Mode {
    // for logic create
    WireItem *wire_item{};

    // for moving while create
    WireNode *current_node{};

    // for new-node-creating
    WireNode *from_node{};
    bool new_node_creating{};

    void reset() {
        wire_item = nullptr;
        current_node = nullptr;

        from_node = nullptr;
        new_node_creating = false;
    }

    ~WireCreating() override {
        reset();
    }
};

struct ComponentMode : Mode {
    ComponentType to_create{};

    std::stack<ComponentItem *> to_delete_stack{};
};

