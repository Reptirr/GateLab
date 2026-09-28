#pragma once
#include <ComponentItem.h>
#include <QGraphicsScene>
#include <WireNode.h>

/*
 * Can:
 *  - create ui component
 *  - remove ui component
 *
 *
 *  - create wire:
 *     - from nothing
 *     - from pin
 *     - from node
 *
 *  - modify wire
 *     - add pin
 *     - remove pin
 *     - collapse node
 *     - create node on wire
 *     - remove node with wire divide
 *
 */

/**
 * rules ui side.
 *
 * directly has scene.
 */
class UIController {
    QGraphicsScene *scene_{};

public:
    explicit UIController(QGraphicsScene *scene) : scene_(scene) {}

    /**
     * add component to scene
     *
     * @param component_item must be blank. component which we paint
     */
    void createComponent(ComponentItem *component_item) {
        my_assert(!scene_->items().contains(component_item));
        for (auto *pin : component_item->pins())
            my_assert(!pin->conn());

        scene_->addItem(component_item);
    }

    /**
     * Delete & disconnects pins from wires. some wires may be deleted after that
     *
     * @param component_item component which we delete
     */
    std::unordered_set<WireItem *> removeComponent(ComponentItem *component_item) {
        my_assert(scene_->items().contains(component_item));

        std::unordered_set<WireItem *> deleted_wires{};

        for (auto *pin : component_item->pins()) {
            if (pin->parentWire()) {
                auto *pin_wire = pin->parentWire();
                if (disconnectPinFromWire(pin_wire, pin))
                    deleted_wires.insert(pin_wire);

            }

            pin->disconnect(); // disconnect from wire. after that some wire may be delete
        }


        delete component_item;

        return deleted_wires;
    }

    /**
     * Create wire. To add pins/nodes call other methods
     */
    WireItem *createWire(WireEndPoint *point1, WireEndPoint *point2) const {
        auto *wire = new WireItem(point1);
        wire->createLine(point1, point2);

        return  wire;
    }

    /**
     * Connects pin to wire with line creating from end point
     *
     * @param wire the wire we join
     * @param from the end point from we create line
     * @param pin the pin which connect
     */
    void connectPinToWire(WireItem *wire, WireEndPoint *from, PinItem *pin) {
        my_assert(wire->contains(from)); // wire has `from` point
        my_assert(pin->lines().empty() && pin->parentWire() == nullptr); // pin does not have connection
        if (auto *from_pin = dynamic_cast<PinItem *>(from))
            my_assert(from_pin->lines().empty()); // if `from` is a pin it must be doesn`t have conn

        wire->createLine(from, pin);
    }

    /**
     * Disconnect pin from wire. After that wire may be invalid.
     *
     * @param wire the wire we leave
     * @param pin the pin we disconnect
     *
     * @return is wire deleted
     */
    bool disconnectPinFromWire(WireItem *wire, PinItem *pin) const {
        my_assert(pin->parentWire() == wire); // pin wire is that wire

        wire->removeEndPoint(pin);

        if (wire->end_points_size() < 2) {
            delete wire; // remove wire & all nodes & disconnects from all pins
            return true;
        }

        return false;
    }

    /**
     * Create node & create line to this node
     *
     * @param wire wire
     * @param from line from point
     * @param pos node pos
     *
     * @return ptr to new node
     */
    WireNode *createNode(WireItem *wire, WireEndPoint *from, QPointF pos) {
        my_assert(wire->contains(from));

        auto *node = new WireNode(wire);
        node->setPos(pos);

        wire->createLine(from, node);

        return node;
    }

    /**
     * Delete node with connect neighbour points.
     *
     * @param wire wire
     * @param node node which we delete. must be with 2 lines
     */
    void collapseNode(WireItem *wire, WireNode *node) {
        if (node->lines().size() != 2) return;
        if (wire->end_points_size() < 3) return;

        wire->collapseNode(node);
    }

    /**
     * create new node in
     *
     * @param line divide
     * @param pos pos in line where will be node
     */
    void divideLineInNode(WireLine *line, QPointF pos) {
        my_assert(line->contains(pos));

        auto *wire_item = line->wire();

        wire_item->divideLine(line, pos);
    }


    void createLine(WireItem *wire_item, WireEndPoint *point_from, WireEndPoint *point_to) {
        my_assert(point_from->parentWire() == wire_item); // point_from is point of that wire

        if (point_to->parentWire() == nullptr) { /* not inited point */
            wire_item->createLine(point_from, point_to);
        } else {
            qDebug() << "createLine to inited point. it`s need unite";
        }
    }

    /**
     * divide wire on other wire and that wire from node.
     *
     * @param wire_item wire
     * @param node from goes dividing
     * @return new wires exclude argument wire
     */
    WireDivideResult divideWireInNode(WireItem *wire_item, WireNode *node) {
        return wire_item->divideWireIn(node);
    }

    void uniteWire(WireItem *first, WireItem *second, WireNode *from, WireNode *to) {
        first->uniteWire(second, from, to);
    }
};