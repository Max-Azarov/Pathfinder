#include <QFont>

#include "gui/sceneItems/AWayPoint.h"
#include "gui/control/Manager.h"


// --------------------------------------------------------------------------------------------------------------------
AWayPoint::AWayPoint(Manager* manager)
    : m_manager(manager)
{
    this->setFont(QFont("Arial", 12));
    this->setText("A");
}
// --------------------------------------------------------------------------------------------------------------------
int AWayPoint::type() const
{ return UserType + Manager::A_WAY_POINT; }
// --------------------------------------------------------------------------------------------------------------------

