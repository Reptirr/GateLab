#pragma once
#include <ComponentItem.h>
#include <LogicComponent.h>
#include <instant/LogicNTransistor.h>
#include <instant/LogicSource.h>
#include <instant/LogicTransistor.h>

inline std::shared_ptr<LogicComponent> logicTypeByItem(const ComponentItem *component_item) {
    switch (component_item->type()) {
        case TransistorType:
            return std::move(LogicComponent::create<LogicTransistor>());

        case SourceType:
            return std::move(LogicComponent::create<LogicSource>());

        case NTransistorType:
            return std::move(LogicComponent::create<LogicNTransistor>());

        default:
            return nullptr; // fallback
    }
}

