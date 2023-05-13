#ifndef BWAYPOINT_H
#define BWAYPOINT_H

#include "gui/sceneItems/AbstractWayPoint.h"


class Manager;

class BWayPoint : public AbstractWayPoint {
public:
    BWayPoint(Manager* manager);
    ~BWayPoint() = default;

    int type() const override;

private:
    Manager* m_manager;
};

#endif // BWAYPOINT_H
