#pragma once
#include <GLCommand.h>
#include <Mode.h>
#include <qgraphicsitem.h>
#include <qgraphicsscene.h>
#include <qpoint.h>

class QMouseEvent;

class InputMapper : public QObject {
    Q_OBJECT

    QGraphicsScene *scene_{};
    ModeType &mode_;

    ComponentItem *deletes_{};

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
    explicit InputMapper(ModeType &mode);

signals:
    void command(GLCommand *command);

public slots:
    void onKeyPress(const QKeyEvent *, QPointF );
    void onKeyRelease(const QKeyEvent*, QPointF pos);
    void onMouseDoubleClick(const QMouseEvent *);
    void onMousePress(const QMouseEvent *);
    void onMouseRelease(const QMouseEvent *);
    void onMouseMove(const QMouseEvent *) const;
};

