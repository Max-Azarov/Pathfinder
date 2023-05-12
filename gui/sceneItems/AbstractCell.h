#ifndef ABSTRACTCELL_H
#define ABSTRACTCELL_H

#include <QGraphicsRectItem>

class AbstractCell : public QGraphicsRectItem {
public:
    int placeNum{};
};

#endif // ABSTRACTCELL_H
