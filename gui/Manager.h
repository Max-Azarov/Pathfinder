#ifndef MANAGER_H
#define MANAGER_H


class Manager
{
public:
    Manager() = default;

private:
    enum { A, B };
    std::array<Cell*, 2> m_ends{};

    bool m_isFirstPaint = true;

    std::vector<Cell*> m_cells{};

    QGraphicsLineItem* m_line;

    QThread* m_pathThread;
    PathFinder* m_pathFinder;

    QGraphicsItem* m_prevItem{};
};

#endif // MANAGER_H
