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
    QGraphicsView::mouseDoubleClickEvent(event);

    qDebug() << "Get double click event";

    emit mouseDoubleClick(event->clone());
}

void MainView::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    emit keyPress(event->clone(), getCursorPosition());
}

void MainView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    emit mousePress(event->clone());
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    emit mouseMove(event->clone());
}

void MainView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);

    scene()->setSceneRect(viewport()->rect());
}


MainView::MainView(QGraphicsScene *main_scene) {
    _main_scene = main_scene;

    setRenderHints({QPainter::Antialiasing, QPainter::TextAntialiasing});
    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);

    setScene(_main_scene);

    // увеличиваем сцену на весь view
    scene()->setSceneRect(viewport()->rect());

    setMouseTracking(true);
}
