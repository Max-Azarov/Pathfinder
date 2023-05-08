#include <QFont>
#include <QDebug>
#include <QMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>


#include "gui/Scene.h"
#include "gui/Cells.h"


// ====================================================================================================================
TextCell::TextCell(QGraphicsItem* parent)
    : QGraphicsSimpleTextItem(parent)
{}
// ====================================================================================================================
ATextCell::ATextCell(QGraphicsRectItem* parent)
    : TextCell(parent)
{
    setFont(QFont("Arial", 12));
    setText("A");
    setPos(parent->rect().center() - boundingRect().center());
    setParentItem(parent);
}
// ====================================================================================================================
BTextCell::BTextCell(QGraphicsRectItem* parent)
    : TextCell(parent)
{
    setFont(QFont("Arial", 12));
    setText("B");
    setPos(parent->rect().center() - boundingRect().center());
    setParentItem(parent);
}
// ====================================================================================================================
// ====================================================================================================================
Cell::Cell(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{}

void Cell::setPlaceNum(int placeNum) { m_placeNum = placeNum; }
int Cell::placeNum() { return m_placeNum; }

void Cell::setPathCell(Cell* cell) { m_pathCell = cell; }
Cell* Cell::pathCell() { return m_pathCell; }
// ====================================================================================================================
WallCell::WallCell(QGraphicsItem* parent)
    : Cell(parent)
{
}
// ====================================================================================================================
MovableCell::MovableCell(QGraphicsItem* parent)
    : Cell(parent)
{
    this->setAcceptHoverEvents(true);
}

void MovableCell::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    Scene* pScene = nullptr;
    try {
        pScene = dynamic_cast<Scene*>(this->scene());
    }
    catch(...) {
        return;
    }

    auto&& childs = this->childItems();

    // set / unset A, B
    if (childs.isEmpty()) {
        if(!pScene->a())  {
            pScene->setA(this);
            new ATextCell(this);
        }
        else if(!pScene->b())  {
            pScene->setB(this);
            new BTextCell(this);

            // finding
            auto const isPathExist = pScene->findPath(pScene->a(), this);
            if (!isPathExist) {
                qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!isPathExist";
            }
        }
    }
    else {
        for (auto child : childs) {
            auto textCell = (QGraphicsSimpleTextItem*)child;
            if (textCell->text() == QString("B")) {
                pScene->setB(nullptr);
                delete child;
                childs.clear();

                pScene->clearLine();
            }
            else if (textCell->text() == QString("A") && !pScene->b()) {
                pScene->setA(nullptr);
                delete child;
                childs.clear();
            }
        }
    }
}

