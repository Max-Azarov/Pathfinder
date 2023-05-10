#ifndef AWAYPOINT_H
#define AWAYPOINT_H

#include "gui/sceneItems/AbstractWayPoint.h"


class Manager;

class AWayPoint : public AbstractWayPoint {
public:
    AWayPoint(Manager* manager)
        : m_manager(manager)
    {}

private:
    Manager* m_manager;
};

#endif // AWAYPOINT_H
