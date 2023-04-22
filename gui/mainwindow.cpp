#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "gui/mainwindow.h"
#include "./ui_mainwindow.h"
#include "gui/Scene.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{}

bool MainWindow::initWindow()
{
    if (ui != nullptr) {
        return true;
    }

    try {
        ui = new Ui::MainWindow;
        ui->setupUi(this);

        auto const colsRows = generate();
        createScene(colsRows.first, colsRows.second);

        QObject::connect(ui->pushButtonGenerate, &QPushButton::released, this, &MainWindow::generate);

        return true;
    }
    catch(...)
    {
        // TODO
    }

    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

std::pair<int, int> MainWindow::generate()
{
    //    qDebug() << __FUNCTION__;

    auto success = true;
    auto ok = false;
    m_width = ui->lineEditWidth->text().toInt(&ok);
    success &= ok;
    m_height = ui->lineEditHeight->text().toInt(&ok);
    success &= ok;

    if (!success) {
        return {};
    }

    return {m_width, m_height};
}


Scene* MainWindow::createScene(int cols, int rows)
{
    auto& view = ui->graphicsView;

    m_scene = new Scene();

    view->setScene(m_scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setWindowTitle("Scene");

    m_scene->init(cols, rows);

    return m_scene;
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    qDebug() << __FUNCTION__;
    qDebug() << ui->graphicsView->size();
    qDebug() << ui->graphicsView->sceneRect();
    ui->graphicsView->setBackgroundBrush(palette().background());
    m_scene->resizeView(event->oldSize(), event->size());
}

