#include <Controller.h>
#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *controller = new Controller();

    controller->mainView()->show();

    return QApplication::exec();
}
