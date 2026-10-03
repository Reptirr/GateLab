#pragma once
#include <Controller.h>
#include <GLCommand.h>
#include <instant/NTransistorItem.h>
#include <instant/SourceItem.h>

inline ComponentItem *componentItemFabric(const ComponentType type) {
    switch (type) {
        case ComponentType::Transistor:
            return new TransistorItem({});
        case ComponentType::NTransistor:
            return new NTransistorItem({});
        case ComponentType::Source:
            return new SourceItem({});
    }
    return new TransistorItem({});
}


class ComponentCreateCommand : public GLCommand {
    QPointF pos_{};
public:
    explicit ComponentCreateCommand(const QPointF center) {
        pos_ = center;
    }

    ModeType needMode() override {
        return ModeType::COMPONENTS;
    }

    void undo() override {
        const auto mode = dynamic_cast<ComponentMode *>(raw_mode_);

        if (mode->to_delete_stack.empty()) return;

        auto *component_item = mode->to_delete_stack.top();
        mode->to_delete_stack.pop();

        controller_->onComponentRemoveRequest(component_item);
    }

    void redo() override {
        const auto mode = dynamic_cast<ComponentMode *>(raw_mode_);

        auto *component_item = componentItemFabric(mode->to_create);
        component_item->setPos(pos_-component_item->rect().center());

        controller_->onComponentCreateRequest(component_item);

        mode->to_delete_stack.push(component_item);
    }

    ~ComponentCreateCommand() override {

    }
};

class ComponentRemoveCommand : public GLCommand {

public:
    explicit ComponentRemoveCommand(ComponentItem *to_delete) {

    }

    ModeType needMode() override {
        return ModeType::COMPONENTS;
    }

    void undo() override {
        const auto mode = dynamic_cast<ComponentMode *>(raw_mode_);

        auto *component_item = componentItemFabric(mode->to_create);
        component_item->setPos(pos_-component_item->rect().center());

        controller_->onComponentCreateRequest(component_item);

        mode->to_delete_stack.push(component_item);
    }

    void redo() override {
        const auto mode = dynamic_cast<ComponentMode *>(raw_mode_);

        if (mode->to_delete_stack.empty()) return;

        auto *component_item = mode->to_delete_stack.top();
        mode->to_delete_stack.pop();

        controller_->onComponentRemoveRequest(component_item);
    }

    ~ComponentRemoveCommand() override {

    }
};
