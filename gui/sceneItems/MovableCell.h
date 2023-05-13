#ifndef MOVABLECELL_H
#define MOVABLECELL_H


#include "gui/sceneItems/AbstractCell.h"


class Manager;

class MovableCell : public AbstractCell {
public:
    MovableCell(Manager* manager, int num);
    ~MovableCell() = default;

    int type() const override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* ) override;

private:
    Manager* m_manager;
};

#endif // MOVABLECELL_H
