#pragma once
#include <ComponentItem.h>
#include <LogicComponent.h>
#include <instant/LogicSource.h>
#include <instant/LogicTransistor.h>

inline LogicComponent *logicTypeByItem(const ComponentItem *component_item) {
    switch (component_item->type()) {
        case TransistorType:
            return new LogicTransistor{};

        case SourceType:
            return new LogicSource{};

        default:
            return nullptr; // fallback
    }
}

