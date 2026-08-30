#include <Controller.h>
#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Controller controller;

    controller.mainView()->show();

    return QApplication::exec();
}
