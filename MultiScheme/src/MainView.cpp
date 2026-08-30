#include <MainView.h>
#include <QMouseEvent>


void MainView::drill_down(ComponentItem *scheme) {

    if (scheme->interior()) {
        qDebug() << "make drill down";
        // push it to stack for drill_up in future
        _drill_stack.push(scene());

        setScene(scheme->interior());
    }
}

void MainView::drill_up() {
    if (!_drill_stack.empty()) {
        qDebug() << "make drill up";
        setScene(_drill_stack.top());
        _drill_stack.pop();
    }
}

QPointF MainView::getCursorPosition() const {
    QPoint viewportPos = viewport()->mapFromGlobal(QCursor::pos());
    QPointF scenePos = mapToScene(viewportPos);

    return scenePos;
}

void MainView::mouseDoubleClickEvent(QMouseEvent *event) {
    qDebug() << "Get double click event";

    QGraphicsItem *selected_item = itemAt(event->pos());

    if (auto *scheme = dynamic_cast<ComponentItem*>(selected_item)) {
        drill_down(scheme);
    }
}

void MainView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        qDebug() << "Get escape press event";

        drill_up();
    } else if (event->key() == Qt::Key_T) {
        QPointF scenePos = getCursorPosition();

        if ( !( 0 <= scenePos.x() && scenePos.x() < scene()->width() ) ) return;
        if ( !( 0 <= scenePos.y() && scenePos.y() < scene()->height() ) ) return;

        qDebug() << "Get t press event at" << scenePos;

        emit key_T_press(scenePos);
    } else if (event->key() == Qt::Key_W) {
        QPointF scenePos = getCursorPosition();

        if ( !( 0 <= scenePos.x() && scenePos.x() < scene()->width() ) ) return;
        if ( !( 0 <= scenePos.y() && scenePos.y() < scene()->height() ) ) return;

        qDebug() << "Get w press event at" << scenePos;

        QList<QGraphicsItem*> items = scene()->items(scenePos);

        for (auto *item : items) {
            if (auto pin = qgraphicsitem_cast<PinItem*>(item)) {
                emit pin_select(pin);
                break;
            }
        }
    }


}

void MainView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);

    scene()->setSceneRect(viewport()->rect());
}


MainView::MainView(QGraphicsScene *main_scene) {
    _main_scene = main_scene;

    setFixedSize(1000, 1000);
    setRenderHints({QPainter::Antialiasing, QPainter::TextAntialiasing});

    setScene(_main_scene);

    // увеличиваем сцену на весь view
    scene()->setSceneRect(viewport()->rect());
}
