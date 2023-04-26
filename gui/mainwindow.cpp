#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QPaintEvent>
#include <QValidator>
#include <QMessageBox>

#include "gui/mainwindow.h"
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

bool MainWindow::initWindow()
{
    if (ui != nullptr) {
        return true;
    }

    try {
        ui = new Ui::MainWindow;
        ui->setupUi(this);

        this->generate();

        QObject::connect(ui->pushButtonGenerate, &QPushButton::released, this, &MainWindow::generate);
        QObject::connect(m_scene, &Scene::setEnabledGenerate, ui->pushButtonGenerate, &QPushButton::setEnabled);

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

        if (m_rowsCols.width() * m_rowsCols.height() > 100 * 100) {
            auto const n = QMessageBox::warning(nullptr,
                                                "Warning",
                                                "Many squares have been queried which may adversely affect performance",
                                                QMessageBox::Ok | QMessageBox::Retry,
                                                QMessageBox::Ok
                                                );
            if (n == QMessageBox::Retry) {
                qDebug() << __FILE__ << ":" << __LINE__ << ":" << "empty";
                ui->lineEditWidth->setText("15");
                ui->lineEditHeight->setText("10");
                tryAgain = true;
            }
        }
    }

    this->createScene();
    this->repaint();
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
        view->setRenderHint(QPainter::Antialiasing);
        //            view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
        //    view->setWindowTitle("Scene");
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

