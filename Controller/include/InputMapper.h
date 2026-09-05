#pragma once
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QObject>
#include <qpoint.h>
#include <stack>

class MainView;
class Controller;
class WireItem;
class ComponentItem;
class QGraphicsScene;
class PinItem;
class QMouseEvent;
class QKeyEvent;

class InputMapper : public QObject {
    Q_OBJECT

    MainView *view_{};
    QGraphicsScene *scene_;

    PinItem *selected_pin_ = nullptr;
    std::stack<QGraphicsScene*> drill_stack_;

    template<typename T>
    T getItem(const QPointF pos) {
        for (auto *item : scene_->items(pos)) {
            if (T res = dynamic_cast<T>(item)) {
                return res;
            }
        }
        return nullptr;
    }

public:
    InputMapper(QGraphicsScene *scene, Controller *controller);

public slots:
    void onKeyPress(const QKeyEvent *, QPointF );
    void onMouseDoubleClick(QMouseEvent *);
    void onMousePress(QMouseEvent *);

    void setScene(QGraphicsScene *);

signals:
    void drillDownRequest(QGraphicsScene *);
    void drillUpRequest(QGraphicsScene *);

    // create requests
    void wireCreateRequest(PinItem *, PinItem *);
    void componentCreateRequest(ComponentItem *);

    // delete request
    void componentRemoveRequest(ComponentItem *);
};

