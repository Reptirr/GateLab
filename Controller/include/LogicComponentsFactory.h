#pragma once
#include <ComponentItem.h>
#include <LogicComponent.h>
#include <instant/LogicSource.h>
#include <instant/LogicTransistor.h>

inline std::shared_ptr<LogicComponent> logicTypeByItem(const ComponentItem *component_item) {
    switch (component_item->type()) {
        case TransistorType:
            return std::move(LogicComponent::create<LogicTransistor>());

        case SourceType:
            return LogicComponent::create<LogicSource>();

        default:
            return nullptr; // fallback
    }
}

