#include <LogicPin.h>
#include <LogicWire.h>
#include <gtest/gtest.h>
#include <instant/LogicSource.h>
#include <instant/LogicTransistor.h>

// Компонент-заглушка ("щуп"): ничего не драйвит сам,
// нужен только чтобы у пина был владелец, но при этом
// пин пассивно принимает значения от провода.
class ProbeComponent : public LogicComponent {
public:
    ProbeComponent()
        : LogicComponent({new LogicPin(this)}) {
    }

    void handle() override {} // ничего не драйвит

    LogicPin* pin() { return pins_[0]; }
};

// Такой же щуп, но с двумя независимыми пинами (не соединены между собой внутри компонента)
class TwoPinProbeComponent : public LogicComponent {
public:
    TwoPinProbeComponent()
        : LogicComponent({new LogicPin(this), new LogicPin(this)}) {
    }

    void handle() override {}

    LogicPin* pinA() { return pins_[0]; }
    LogicPin* pinB() { return pins_[1]; }
};


TEST(WireTest, WireTransmitsSignal) {
    TwoPinProbeComponent component;
    LogicWire wire;

    component.pinA()->setWire(&wire);
    component.pinB()->setWire(&wire);
    wire.addPin(component.pinA());
    wire.addPin(component.pinB());

    component.pinA()->setSignalByOwner(true);

    ASSERT_TRUE(component.pinB()->getSignal());
}

TEST(WireTest, WireIsOrOfAllOwnSignals) {
    ProbeComponent a, b, c;
    LogicWire wire;

    for (auto* p : {a.pin(), b.pin(), c.pin()}) {
        p->setWire(&wire);
        wire.addPin(p);
    }

    // никто не драйвит -> false
    ASSERT_FALSE(a.pin()->getSignal());

    // b начинает драйвить true -> все видят true
    b.pin()->setSignalByOwner(true);
    ASSERT_TRUE(a.pin()->getSignal());
    ASSERT_TRUE(c.pin()->getSignal());

    // b перестаёт драйвить -> все обратно false
    b.pin()->setSignalByOwner(false);
    ASSERT_FALSE(a.pin()->getSignal());
    ASSERT_FALSE(c.pin()->getSignal());
}

TEST(WireTest, RemovePinRecalculatesSignal) {
    ProbeComponent driver, listener;
    LogicWire wire;

    driver.pin()->setWire(&wire);
    listener.pin()->setWire(&wire);
    wire.addPin(driver.pin());
    wire.addPin(listener.pin());

    driver.pin()->setSignalByOwner(true);
    ASSERT_TRUE(listener.pin()->getSignal());

    driver.pin()->removeWire();
    wire.removePin(driver.pin());

    // драйвера больше нет на проводе -> сигнал должен упасть
    ASSERT_FALSE(listener.pin()->getSignal());
}

TEST(TransistorTest, OutputsAndOfInputs) {
    LogicTransistor transistor;
    auto [left, top, right] = transistor.pinsTuple();

    left->setSignalByWire(false);
    top->setSignalByWire(false);
    ASSERT_FALSE(right->getSignal());

    left->setSignalByWire(true);
    top->setSignalByWire(false);
    ASSERT_FALSE(right->getSignal());

    left->setSignalByWire(true);
    top->setSignalByWire(true);
    ASSERT_TRUE(right->getSignal());
}

TEST(SourceTest, AlwaysOutputsTrue) {
    LogicSource source;

    ASSERT_TRUE(source.pins()[0]->getSignal());
}

TEST(SourceTest, StaysTrueWhenConnectedToWireWithOtherPin) {
    LogicSource source;
    ProbeComponent listener;
    LogicWire wire;

    source.pins()[0]->setWire(&wire);
    listener.pin()->setWire(&wire);
    wire.addPin(source.pins()[0]);
    wire.addPin(listener.pin());

    ASSERT_TRUE(source.pins()[0]->getSignal());
    ASSERT_TRUE(listener.pin()->getSignal());
}