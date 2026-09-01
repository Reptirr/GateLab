#include <LogicPin.h>
#include <LogicWire.h>
#include <gtest/gtest.h>
#include <instant/LogicTransistor.h>

class ComponentMoc : LogicComponent {
public:

    void handle() override {}

    std::vector<LogicPin *> pins() override { return {new LogicPin(this), new LogicPin(this)}; }
};

TEST(WireTest, WireTransmitsSignal) {
    auto *component = new ComponentMoc();

    auto pin1 = component->pins()[0];
    auto pin2 = component->pins()[1];

    auto *wire = new LogicWire();

    // говорим пинам что кому должны говорить что они изменились
    pin1->setWire(wire);
    pin2->setWire(wire);

    // говорим вайру кого он соединяет
    wire->addPin(pin1);
    wire->addPin(pin2);

    pin1->setSignalByOwner(true);

    ASSERT_TRUE(pin2->getSignal());

}

TEST(Transistor, TransistorWork) {
    auto *transistor = new LogicTransistor();

    auto [left, top, right] = transistor->pinsTuple();

    left->setSignalByWire(true);
    top->setSignalByWire(true);

    ASSERT_TRUE(right->getSignal());
}
