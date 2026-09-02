#pragma once
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <QObject>
#include <qpoint.h>
#include <stack>

class WireItem;
class ComponentItem;
class QGraphicsScene;
class PinItem;
class QMouseEvent;
class QKeyEvent;

class InputMapper : public QObject {
    Q_OBJECT

    QGraphicsScene *scene_;

    PinItem *selected_pin_ = nullptr;
    std::stack<QGraphicsScene*> drill_stack_;

    template<typename T>
    T getItem(const QPointF pos) {
        for (auto *item : scene_->items(pos)) {
            if (T res = qgraphicsitem_cast<T>(item)) {
                return res;
            }
        }

        return nullptr;
    }

public:
    explicit InputMapper(QGraphicsScene *);

public slots:
    void onKeyPress(QKeyEvent *, QPointF );
    void onMouseDoubleClick(QMouseEvent *);

    void setScene(QGraphicsScene *);

signals:
    void drillDownRequest(QGraphicsScene *);
    void drillUpRequest(QGraphicsScene *);

    // create requests
    void wireCreateRequest(PinItem *, PinItem *);
    void transistorCreateRequest(QPointF);
    void sourceCreateRequest(QPointF);

    // delete request
    void componentRemoveRequest(ComponentItem *);
};

