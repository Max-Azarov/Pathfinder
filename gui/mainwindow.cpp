#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPaintEvent>

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

        generate();

        QObject::connect(ui->pushButtonGenerate, &QPushButton::released, this, &MainWindow::generate);

        if (m_rowsCols.isEmpty()) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
            return false;
        }

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

void MainWindow::generate()
{
    auto success = true;
    auto ok = false;
    m_rowsCols.setWidth(ui->lineEditWidth->text().toInt(&ok));
    success &= ok;
    m_rowsCols.setHeight(ui->lineEditHeight->text().toInt(&ok));
    success &= ok;

    if (!success) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
        m_rowsCols = QSize{};
        return;
    }

    createScene();
}


void MainWindow::createScene()
{
    if (m_rowsCols.isEmpty()) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
        return;
    }

    auto& view = ui->graphicsView;
    if (!m_scene) {
        m_scene = new Scene();

        view->setScene(m_scene);
        //    view->setRenderHint(QPainter::Antialiasing);
        //    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        view->setWindowTitle("Scene");
        view->setBackgroundBrush(palette().background());
    }

    m_scene->init(m_rowsCols.width(), m_rowsCols.height());
}

void MainWindow::paintEvent(QPaintEvent*)
{
    m_scene->initView();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    m_scene->resizeView(event->oldSize(), event->size());
}

