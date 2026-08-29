#include <LogicPin.h>
#include <LogicTransistor.h>
#include <LogicWire.h>
#include <gtest/gtest.h>

TEST(WireTest, WireTransmitsSignal) {
    auto *pin1 = new LogicPin();
    auto *pin2 = new LogicPin();

    auto *wire = new LogicWire();

    // говорим пинам что кому должны говорить что они изменились
    pin1->addHandler(wire);
    pin2->addHandler(wire);

    // говорим вайру кого он соединяет
    wire->addPin(pin1);
    wire->addPin(pin2);

    pin1->setSignal(true);

    ASSERT_TRUE(pin2->getSignal());

}

TEST(Transistor, TransistorWork) {
    auto *left = new LogicPin();
    auto *top = new LogicPin();
    auto *right = new LogicPin();

    auto *transistor = new LogicTransistor(left, top, right);

    left->addHandler(transistor);
    top->addHandler(transistor);

    left->setSignal(true);
    top->setSignal(true);

    ASSERT_TRUE(right->getSignal());
}