#include "mainwindow.h"

#include <QApplication>
#include <QDebug>

#include "core/include/IWorker.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto worker = IWorker::create();
    if (worker) {
        qDebug() << worker.get();

        auto result = worker->find_path(7, 24);
        qDebug() << result;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
