#ifndef PATHLINE_H
#define PATHLINE_H

#include <QGraphicsLineItem>

class Manager;

class PathLine : public QGraphicsLineItem {
public:
    PathLine(Manager* manager)
        : m_manager(manager)
    {}

    int type() const override;

private:
    Manager* m_manager;
};

#endif // PATHLINE_H
