#include <QPen>

#include "gui/sceneItems/WallCell.h"
#include "gui/control/Manager.h"


// --------------------------------------------------------------------------------------------------------------------
WallCell::WallCell(Manager* manager, int num)
    : m_manager(manager)
{
    placeNum = num;
    this->setBrush(QBrush(Qt::red));
    this->setPen(QPen(Qt::darkGray));
}
// --------------------------------------------------------------------------------------------------------------------
int WallCell::type() const
{ return UserType + Manager::WALL_CELL; }
 // --------------------------------------------------------------------------------------------------------------------
