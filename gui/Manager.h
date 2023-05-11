#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <cstdint>
#include <memory>

#include <QObject>
#include <QThread>

#include "gui/VectorHolder.h"
#include "gui/sceneItems/PathLine.h"
#include "gui/sceneItems/AbstractCell.h"
#include "gui/sceneItems/AbstractWayPoint.h"

class QThread;

class AbstractCell;
class AbstractWayPoint;
class PathFinder;
class PathLine;
class Scene;
class Field;


class Manager : public QObject
{
    Q_OBJECT
public:
    Manager(Scene* );
    Manager() = delete;
    ~Manager() = default;

    void onClicked(AbstractCell*) noexcept;
    void onClicked(AbstractWayPoint*) noexcept;

signals:
    void initScene(int, int);
    void calcPath(AbstractCell*, AbstractCell*);

public slots:
    void initCells(const Field*);
    void foundPath(VectorHolder<int>* wrappedPath, int start, int goal);

private:
    void initThread() noexcept;
    void destroyThread() noexcept;

private:
    struct {
        std::uint8_t a : 1;
        std::uint8_t b : 1;
        std::uint8_t   : 6;
    } m_state{};

    PathFinder* m_pathFinder = nullptr;
    std::unique_ptr<QThread> m_pathThread = nullptr;
    std::unique_ptr<PathLine> m_pathLine = nullptr;
    std::vector<std::unique_ptr<AbstractCell>> m_field{};
    std::vector<std::unique_ptr<AbstractWayPoint>> m_wayPoints{};
};

#endif // MANAGER_H
