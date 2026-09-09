#include <Controller.h>
#include <MainWindow.h>
#include <QApplication>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    auto *mainWindow = new MainWindow{};

    mainWindow->show();

    return QApplication::exec();
}
