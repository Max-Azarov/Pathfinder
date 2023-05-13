
#include "gui/sceneItems/BWayPoint.h"
#include "gui/Scene.h"


// --------------------------------------------------------------------------------------------------------------------
BWayPoint::BWayPoint(Manager* manager)
    : m_manager(manager)
{
    this->setFont(QFont("Arial", 12));
    this->setText("B");
}
// --------------------------------------------------------------------------------------------------------------------
int BWayPoint::type() const
{ return UserType + Manager::B_WAY_POINT; }
 // --------------------------------------------------------------------------------------------------------------------

