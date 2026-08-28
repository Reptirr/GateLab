#include <MainView.h>
#include <QMouseEvent>


void MainView::drill_down(const SchemeItem *scheme) {

    if (scheme->_interior) {
        qDebug() << "make drill down";
        // push it to stack for drill_up in future
        _drill_stack.push(scene());

        setScene(scheme->_interior);
    }
}

void MainView::drill_up() {
    if (!_drill_stack.empty()) {
        qDebug() << "make drill up";
        setScene(_drill_stack.top());
        _drill_stack.pop();
    }
}

void MainView::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << "Get double click event";

    QGraphicsItem *selected_item = itemAt(event->pos());

    if (const auto *scheme = dynamic_cast<SchemeItem*>(selected_item)) {
        drill_down(scheme);
    }
}

void MainView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Get escape press event";

        drill_up();
    }
}


MainView::MainView(QGraphicsScene *main_scene) {
    _main_scene = main_scene;

    setScene(_main_scene);
}
