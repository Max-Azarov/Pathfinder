#ifndef AWAYPOINT_H
#define AWAYPOINT_H

#include "gui/sceneItems/AbstractWayPoint.h"


class Manager;

class AWayPoint : public AbstractWayPoint {
public:
    AWayPoint(Manager* manager);
    ~AWayPoint() = default;

    int type() const override;

private:
    Manager* m_manager;
};

#endif // AWAYPOINT_H
