#pragma once

#include <QGraphicsView>
#include <SchemeItem.h>
#include <stack>

class MainView : public QGraphicsView {
    QGraphicsScene *_main_scene{};

    std::stack<QGraphicsScene*> _drill_stack;


    void drill_down(const SchemeItem *scheme);
    void drill_up();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override; // for drill-down
    void keyPressEvent(QKeyEvent *event) override; // for drill-up

public:
    explicit MainView(QGraphicsScene *main_scene);

};
