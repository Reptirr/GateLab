#pragma once

#include <QGraphicsView>
#include <stack>
#include <ComponentItem.h>

class MainView : public QGraphicsView {
    Q_OBJECT

    QGraphicsScene *_main_scene{};

    std::stack<QGraphicsScene*> _drill_stack;


    void drill_down(ComponentItem *scheme);
    void drill_up();

    QPointF getCursorPosition() const;

signals:
    void keyPress(QKeyEvent *, QPointF);
    void mouseDoubleClick(QMouseEvent *);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override; // for drill-down
    void keyPressEvent(QKeyEvent *event) override; // for creating & drill-down

    void resizeEvent(QResizeEvent *event) override; // for scene rect resize

public:
    explicit MainView(QGraphicsScene *main_scene);

};
