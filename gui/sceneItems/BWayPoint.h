#ifndef BWAYPOINT_H
#define BWAYPOINT_H

#include "gui/sceneItems/AbstractWayPoint.h"


class Manager;

class BWayPoint : public AbstractWayPoint {
public:
    BWayPoint(Manager* manager)
        : m_manager(manager)
    {}

private:
    Manager* m_manager;
};

#endif // BWAYPOINT_H
