#ifndef WALLCELL_H
#define WALLCELL_H

#include <QPen>

#include "gui/sceneItems/AbstractCell.h"


class Manager;

class WallCell : public AbstractCell {
public:
    WallCell(Manager* manager, int placeNum_)
        : m_manager(manager)
    {
        placeNum = placeNum_;
        this->setBrush(QBrush(Qt::red));
        this->setPen(QPen(Qt::darkGray));
    }

private:
    Manager* m_manager;
};

#endif // WALLCELL_H
