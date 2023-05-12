#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <cstdint>
#include <memory>

#include <QObject>
#include <QThread>


class QThread;

class AbstractCell;
class AbstractWayPoint;
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
    void onClicked(AbstractWayPoint*) noexcept;
    bool init(int cols, int rows) noexcept;
    void clear() noexcept;

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

    union {
        std::uint8_t common;
        struct {
            std::uint8_t a : 1;
            std::uint8_t b : 1;
            std::uint8_t   : 6;
        } value;
    } m_state{};

    std::unique_ptr<PathFinder> m_pathFinder{};
    std::unique_ptr<QThread> m_pathThread{};
    std::unique_ptr<PathLine> m_pathLine{};
    std::vector<std::unique_ptr<AbstractCell>> m_field{};
    std::vector<std::unique_ptr<AbstractWayPoint>> m_wayPoints{};
};

#endif // MANAGER_H
