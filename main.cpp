#include <QApplication>
#include <QDebug>

#include "gui/mainwindow.h"
#include "core/include/Worker.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    auto worker = Worker::create();
    if (!worker) {
        qDebug() << __FUNCTION__ << ":" << __LINE__;
        return EXIT_FAILURE;
    }

    MainWindow w{};
    if (!w.initWindow()) {
        qDebug() << __FUNCTION__ << ":" << __LINE__;
        return EXIT_FAILURE;
    }






    w.show();

    //    QObject::connect(&w, )

    //    auto response = QObject::connect(&w::lineEditWidth, &QLineEdit::editingFinished),
    //            &worker, )
    //            1;

    return a.exec();

}
