#include <MainView.h>
#include <QMouseEvent>


QPointF MainView::getCursorPosition() const {
    QPoint viewportPos = viewport()->mapFromGlobal(QCursor::pos());
    QPointF scenePos = mapToScene(viewportPos);

    return scenePos;
}

void MainView::mouseDoubleClickEvent(QMouseEvent *event) {
    QGraphicsView::mouseDoubleClickEvent(event);

    emit MouseDoubleClick(event);
}

void MainView::keyPressEvent(QKeyEvent *event) {
    QGraphicsView::keyPressEvent(event);
    emit KeyPress(event, getCursorPosition());
}

void MainView::keyReleaseEvent(QKeyEvent *event) {
    QGraphicsView::keyReleaseEvent(event);
    emit KeyRelease(event, getCursorPosition());
}

void MainView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    emit MousePress(event);
}

void MainView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    emit MouseMove(event);
}

void MainView::mouseReleaseEvent(QMouseEvent *event) {
    emit MouseRelease(event);
}

void MainView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);

    scene()->setSceneRect(viewport()->rect());
}

MainView::MainView() {
    setRenderHints({QPainter::TextAntialiasing});
    setViewportUpdateMode(BoundingRectViewportUpdate);

    viewport()->setAttribute(Qt::WA_AcceptTouchEvents, false);

    setScene(new QGraphicsScene());

    // увеличиваем сцену на весь view
    scene()->setSceneRect(viewport()->rect());

    setMouseTracking(true);

}
