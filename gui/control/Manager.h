#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <cstdint>
#include <memory>
#include <stack>

#include <QObject>
#include <QThread>

#include "gui/sceneItems/AbstractWayPoint.h"


class QThread;

class AbstractCell;
class PathFinder;
class PathLine;
class Scene;
class Field;


template<class T>
class VectorHolder;


class Manager : public QObject
{
    Q_OBJECT
public:
    Manager(Scene* );
    Manager() = delete;
    ~Manager();

    void onClicked(AbstractCell*) noexcept;
    bool init(int cols, int rows) noexcept;
    void clear() noexcept;

    enum ItemType {
        WALL_CELL = 1,
        MOVABLE_CELL,
        PATH_LINE,
        A_WAY_POINT,
        B_WAY_POINT,
    };

signals:
    void initScene(int, int);
    void calcPath(AbstractCell*, AbstractCell*);

public slots:
    void initCells(const Field*);
    void foundPath(VectorHolder<int>* wrappedPath, int start, int goal);

private:
    void initThread();
    void destroyThread() noexcept;

private:
    Scene* m_scene = nullptr;

    std::unique_ptr<PathFinder> m_pathFinder{};
    std::unique_ptr<QThread> m_pathThread{};

    std::vector<std::unique_ptr<AbstractCell>> m_field{};
    std::vector<std::unique_ptr<PathLine>> m_pathLine{};

    std::stack<std::unique_ptr<AbstractWayPoint>> m_wayPoints{};
};

#endif // MANAGER_H
