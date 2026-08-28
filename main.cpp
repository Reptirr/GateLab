#include <MainView.h>
#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *main_scene = new QGraphicsScene();
    main_scene->setBackgroundBrush(QBrush(Qt::white));



    auto *main_view = new MainView(main_scene);
    main_view->setFixedSize(1000, 1000);


    main_view->show();



    return QApplication::exec();
}
