#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "core/include/Worker.h"

class QGraphicsScene;
class QGraphicsView;
class QGraphicsRectItem;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneMouseEvent;
class QGraphicsSimpleTextItem;

class Worker;


// ====================================================================================================================
class ATextCell : public QGraphicsSimpleTextItem
{
public:
    ATextCell(QGraphicsRectItem* parent = nullptr);
}; // class
// ====================================================================================================================
class BTextCell : public QGraphicsSimpleTextItem
{
public:
    BTextCell(QGraphicsRectItem* parent = nullptr);
}; // class
// ====================================================================================================================
class Cell : public QGraphicsRectItem
{
public:
    Cell(QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
}; // class
// ====================================================================================================================
class WallCell : public QGraphicsRectItem
{
public:
    WallCell(QGraphicsItem* parent = nullptr);
}; // class
// ====================================================================================================================
class Scene : public QGraphicsScene
{
public:
    Scene() = default;

    QSize init(int cols, int rows);
    void initView();
    void resizeView(const QSize& oldSize, const QSize& size);

    bool setA();
    bool unsetA();
    bool isASet();

    bool setB();
    bool unsetB();
    bool isBSet();

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    static int constexpr m_cellSize = 15;
    int m_rows{};
    int m_cols{};

    bool m_isFirstPaint = true;
    bool m_isACellSet = false;
    bool m_isBCellSet = false;

    std::unique_ptr<Worker> m_worker;
};
// ====================================================================================================================

#endif // SCENE_H
