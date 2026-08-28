#pragma once
#include <MainView.h>
#include <PinItem.h>

class Controller {
    MainView *main_view_{};

public:
    Controller() {
        main_view_ = new MainView(new QGraphicsScene());
    }

    void addWire(PinItem* pin1, PinItem* pin2) {
        // pins must be
        if (pin1 == nullptr || pin2 == nullptr) return;

        // pins must be on same drill-levels
        if (pin1->scene() != pin2->scene()) return;

        // connecting
        main_view_->scene()->addItem(new WireItem)

    }
};
