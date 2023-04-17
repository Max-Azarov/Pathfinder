#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

#include "core/include/IWorker.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto worker = IWorker::create();
    if (worker) {
        qDebug("worker");
    }

    MainWindow w;
    w.show();
    return a.exec();
}
