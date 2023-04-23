#include <QApplication>
#include <QDebug>

#include "gui/mainwindow.h"
//#include "core/include/Worker.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w{};
    if (!w.initWindow()) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "EXIT_FAILURE";
        return EXIT_FAILURE;
    }

    w.show();

    return a.exec();
}
