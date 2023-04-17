#include "mainwindow.h"

#include <QApplication>

#include "core/include/Broker.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



    MainWindow w;
    w.show();
    return a.exec();
}
