#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <array>
#include <vector>

#include "core/include/Worker.h"

class QGraphicsScene;
class QGraphicsView;
class QGraphicsItem;
class QGraphicsRectItem;
class QGraphicsLineItem;
class QGraphicsSimpleTextItem;
class QGraphicsSceneWheelEvent;
class QGraphicsSceneMouseEvent;

class Worker;


// ====================================================================================================================
class TextCell : public QGraphicsSimpleTextItem
{
public:
    TextCell(QGraphicsItem* parent = nullptr);
}; // class
// ====================================================================================================================
class ATextCell : public TextCell
{
public:
    ATextCell(QGraphicsRectItem* parent = nullptr);
}; // class
// ====================================================================================================================
class BTextCell : public TextCell
{
public:
    BTextCell(QGraphicsRectItem* parent = nullptr);
}; // class
// ====================================================================================================================
// ====================================================================================================================
class Cell : public QGraphicsRectItem
{
public:
    Cell(QGraphicsItem* parent = nullptr);
    void setPlaceNum(int);
    int placeNum();

    void setPathCell(Cell*);
    Cell* pathCell();

private:
    int m_placeNum{-1};
    Cell* m_pathCell{};
}; // class
// ====================================================================================================================
class MovableCell : public Cell
{
public:
    MovableCell(QGraphicsItem* parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
}; // class
// ====================================================================================================================
class WallCell : public Cell
{
public:
    WallCell(QGraphicsItem* parent = nullptr);
}; // class
// ====================================================================================================================
class Scene : public QGraphicsScene
{
public:
    Scene() = default;

    bool init(int cols, int rows) noexcept;
    void initView();
    void resizeView(const QSize& oldSize, const QSize& size);
    bool findPath(Cell* start, Cell* goal);

    void setA(Cell*);
    Cell* a() const;

    void setB(Cell*);
    Cell* b() const;

    void clearLine();

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    void createLine(int goal_idx);

private:
    static int constexpr m_cellSize = 15;
    int m_rows{};
    int m_cols{};

    enum { A, B };
    std::array<Cell*, 2> m_ends{};

    bool m_isFirstPaint = true;

    std::unique_ptr<Worker> m_worker;

    std::vector<Cell*> m_cells{};

    QGraphicsLineItem* m_line;
};
// ====================================================================================================================

#endif // SCENE_H
