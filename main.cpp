#include <iostream>
#include <LogicComponent.h>
#include <LogicPin.h>
#include <LogicWire.h>
#include <SchemeItem.h>

#include <QApplication>
#include <qgraphicsscene.h>
#include <QStaticText>

class Transistor : public SchemeItem {

    qreal width_ = 150;
    qreal height_ = 100;

public:
    QRectF boundingRect() const override {
        return QRectF{0, 0, width_, height_};
    }


    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QPen pen(QColorConstants::Black);
        pen.setWidth(2);
        painter->setPen(pen);

        // paint a border
        painter->drawRect(0, 0, width_, height_);

        // paint a title
        const QStaticText text("Transistor");

        painter->drawStaticText(
            (width_ - text.size().width()) / 2,
            (height_ - text.size().height()) / 2,
            text
        );

        // paint a pins
        paintPins(painter);

    }

protected:
    std::vector<PinItem *> pins() override {
        auto *leftPin = new PinItem();
        leftPin->setPos(
            -10,
            (height_ - leftPin->boundingRect().height())/2
        );

        return {
            leftPin
        };
    };
};

class GateAnd : public SchemeItem {
    qreal _width = 150;
    qreal _height = 100;

public:

    GateAnd() {
        _interior = new QGraphicsScene();

        // gate AND has two transistors

        auto *trs1 = new Transistor();
        trs1->setPos(50, 50);

        auto *trs2 = new Transistor();
        trs2->setPos(50, 500);

        _interior->addItem(trs1);
        _interior->addItem(trs2);
    }


    QRectF boundingRect() const override {
        return QRectF{0, 0, _width, _height};
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        QPen pen(QColorConstants::Black);
        pen.setWidth(2);
        painter->setPen(pen);
        // paint a border
        painter->drawRect(0, 0, _width, _height);

        // paint a title
        QStaticText text("Gate And");
        painter->drawStaticText(
            (_width - text.size().width()) / 2,
            (_height - text.size().height()) / 2,
            text
        );
    }

protected:
    std::vector<PinItem *> pins() override {
        return {};
    };
};

class LogicTransistor : public LogicComponent {
    LogicPin* left_pin_;
    LogicPin* top_pin_;
    LogicPin* right_pin_;
public:
    LogicTransistor(LogicPin* left_pin, LogicPin* top_pin, LogicPin* right_pin) : LogicComponent({}), left_pin_(left_pin),
        top_pin_(top_pin), right_pin_(right_pin) {
    }


    void handle() override {
        if (top_pin_->getSignal()) {
            right_pin_->setSignal(left_pin_->getSignal());
        } else {
            left_pin_->setSignal(false);
        }
    }
};


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // auto *main_scene = new QGraphicsScene();
    // main_scene->setBackgroundBrush(QBrush(Qt::white));
    //
    // auto *gate = new GateAnd();
    // main_scene->addItem(gate);
    //
    // auto *main_view = new MainView(main_scene);
    // main_view->setFixedSize(1000, 1000);
    //
    //
    // main_view->show();



    return QApplication::exec();
}
