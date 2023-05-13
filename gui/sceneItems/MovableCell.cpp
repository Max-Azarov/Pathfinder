#include <QPen>

#include "gui/sceneItems/MovableCell.h"
#include "gui/control/Manager.h"


// --------------------------------------------------------------------------------------------------------------------
MovableCell::MovableCell(Manager* manager, int num)
    : m_manager(manager)
{
    placeNum = num;
    this->setPen(QPen(Qt::darkGray));
    this->setAcceptHoverEvents(true);
}
// --------------------------------------------------------------------------------------------------------------------
 int MovableCell::type() const
 { return UserType + Manager::MOVABLE_CELL; }
 // --------------------------------------------------------------------------------------------------------------------
 void MovableCell::mousePressEvent(QGraphicsSceneMouseEvent*)
 {
     m_manager->onClicked(this);
 }
// --------------------------------------------------------------------------------------------------------------------
