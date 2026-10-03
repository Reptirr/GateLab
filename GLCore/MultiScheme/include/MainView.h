#pragma once

#include <QGraphicsView>
#include <stack>
#include <ComponentItem.h>

class MainView : public QGraphicsView {
    Q_OBJECT


    QPointF getCursorPosition() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

signals:
    void KeyPress(const QKeyEvent *, QPointF);
    void KeyRelease(const QKeyEvent *, QPointF);
    void MouseDoubleClick(const QMouseEvent *);
    void MousePress(const QMouseEvent *);
    void MouseRelease(const QMouseEvent *);
    void MouseMove(const QMouseEvent *);

public:
    MainView();



};
