#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Scene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    std::pair<int, int> generate();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool initWindow();

protected:
    void resizeEvent(QResizeEvent* event) override;


private:
    Scene* createScene(int width, int height);

private:
    Ui::MainWindow* ui{};
    Scene* m_scene;
    int m_width{};
    int m_height{};

}; // class

#endif // MAINWINDOW_H
