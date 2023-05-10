#ifndef MOVABLECELL_H
#define MOVABLECELL_H

#include "gui/sceneItems/AbstractCell.h"


class Manager;

class MovableCell : public AbstractCell {
public:
    MovableCell(Manager* manager)
        : m_manager(manager)
    {}

private:
    Manager* m_manager;
};

#endif // MOVABLECELL_H
