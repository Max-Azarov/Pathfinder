#ifndef PATHLINE_H
#define PATHLINE_H


class Manager;

class PathLine {
public:
    PathLine(Manager* manager)
        : m_manager(manager)
    {}

private:
    Manager* m_manager;
};

#endif // PATHLINE_H
