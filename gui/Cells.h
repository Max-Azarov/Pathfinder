#ifndef CELLS_H
#define CELLS_H


#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsSceneMouseEvent>


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

#endif // CELLS_H
