#ifndef MOVABLECELL_H
#define MOVABLECELL_H

#include <QPen>

#include "gui/sceneItems/AbstractCell.h"


class Manager;

class MovableCell : public AbstractCell {
public:
    MovableCell(Manager* manager, int placeNum_)
        : m_manager(manager)
    {
        placeNum = placeNum_;
        this->setPen(QPen(Qt::darkGray));
    }
    ~MovableCell() = default;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* ) override;

private:
    Manager* m_manager;
};

#endif // MOVABLECELL_H
