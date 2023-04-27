#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QCloseEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void generate();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool initWindow();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent* event) override;


private:
    void createScene();

private:
    Ui::MainWindow* ui = nullptr;
    Scene* m_scene = nullptr;
    QSize m_rowsCols{};

}; // class

#endif // MAINWINDOW_H
