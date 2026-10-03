#pragma once
#include <QPointF>

class MoveHelper {
    QPointF last_mouse_pos_{};

public:
    void onMousePress(QPointF pos) {
        last_mouse_pos_ = pos;
    }

    QPointF onMouseMove(const QPointF current_pos, const QPointF pos) {
        const QPointF delta = pos - last_mouse_pos_;
        last_mouse_pos_ = pos;
        return current_pos+delta;
    }
};