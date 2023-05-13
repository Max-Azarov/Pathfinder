#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include <QValidator>
#include <QMessageBox>
#include <QSettings>
#include <QCloseEvent>

#include "gui/mainwindow.h"
#include "gui/MyGraphicsView.h"
#include "./ui_mainwindow.h"
#include "gui/Scene.h"


namespace  {

class WidthHeightValidator : public QValidator
{
public:
    WidthHeightValidator(QObject* parent)
        : QValidator(parent)
    {}

    State validate(QString& str, int&) const override
    {
        QRegExp rxp = QRegExp("([1-9][0-9]{0,3})?");
        if (str.contains(rxp) && str.size() == rxp.matchedLength()) {
            return State::Acceptable;
        }
        return State::Invalid;
    }
}; // class

} // namespace


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{}


void MainWindow::closeEvent(QCloseEvent* event)
{
    // Сохранение настроек в конфигурационный файл
    QSettings settings("MyCompany", "pathfinder");
    settings.setValue("pos", this->pos());
    settings.setValue("size", this->size());

    QMainWindow::closeEvent(event);
}


bool MainWindow::initWindow()
{
    if (ui != nullptr) {
        return true;
    }

    try {
        ui = new Ui::MainWindow;
        ui->setupUi(this);

        // Загрузка настроек из конфигурационного файла
        QSettings settings("MyCompany", "pathfinder");
        QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
        QSize size = settings.value("size", QSize(400, 400)).toSize();
        this->resize(size);
        this->move(pos);

        this->generate();

        QObject::connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &MainWindow::generate);
        QObject::connect(m_scene, &Scene::setEnabledGenerate, ui->pushButtonGenerate, &QPushButton::setEnabled);

        if (m_rowsCols.isEmpty()) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
            return false;
        }

        return true;
    }
    catch(...) {}
    qDebug() << __FILE__ << ":" << __LINE__ << ":" << "catch(...) {}";

    return false;
}

MainWindow::~MainWindow()
{
    delete m_scene;
    delete ui;
}

void MainWindow::generate()
{
    auto tryAgain = true;

    while(tryAgain) {
        tryAgain = false;

        ui->lineEditWidth->setValidator(new WidthHeightValidator(ui->lineEditWidth));
        ui->lineEditHeight->setValidator(new WidthHeightValidator(ui->lineEditHeight));

        auto success = true;
        auto ok = false;
        m_rowsCols.setWidth(ui->lineEditWidth->text().toInt(&ok));
        success &= ok;
        m_rowsCols.setHeight(ui->lineEditHeight->text().toInt(&ok));
        success &= ok;

        success &= m_rowsCols.width() > 0 ? true : false;
        success &= m_rowsCols.height() > 0 ? true : false;

        if (!success) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
            m_rowsCols = QSize{};
            return;
        }

        if (m_rowsCols.width() * m_rowsCols.height() > 1000 * 1000) {
            auto const n = QMessageBox::warning(this,
                                                "Warning",
                                                "Many squares have been queried which may adversely affect performance",
                                                QMessageBox::Ok | QMessageBox::Retry,
                                                QMessageBox::Ok
                                                );
            if (n == QMessageBox::Retry) {
                ui->lineEditWidth->setText("15");
                ui->lineEditHeight->setText("10");
                tryAgain = true;
            }
        }
    }

    this->createScene();
}


void MainWindow::createScene()
{
    if (m_rowsCols.isEmpty()) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
        return;
    }

    auto view = ui->graphicsView;

    if (!m_scene) {
        view->setCacheMode(QGraphicsView::CacheNone);

        m_scene = new Scene();

        view->setScene(m_scene);
        view->setRenderHint(QPainter::Antialiasing);
        view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        view->setBackgroundBrush(palette().background());
    }

    m_scene->init(m_rowsCols.width(), m_rowsCols.height());
    view->update();
    view->repaint();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    m_scene->initView();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    m_scene->resizeView(event->oldSize(), event->size());
}

