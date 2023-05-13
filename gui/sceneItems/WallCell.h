#ifndef WALLCELL_H
#define WALLCELL_H


#include "gui/sceneItems/AbstractCell.h"


class Manager;

class WallCell : public AbstractCell {
public:
    WallCell(Manager* manager, int num);
    ~WallCell() = default;

    int type() const override;

private:
    Manager* m_manager;
};

#endif // WALLCELL_H
