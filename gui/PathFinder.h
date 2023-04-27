#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <memory>
#include <vector>

#include <QThread>
#include <QGraphicsScene>
#include <QDebug>

#include "core/include/Worker.h"
#include "gui/Cells.h"
#include "gui/Scene.h"
#include "gui/VectorHolder.h"




class PathFinderThread : public QThread {
public:
    void run() override {
        this->exec();
    }
}; // class


class PathFinder : public QObject {
    Q_OBJECT
public:
    PathFinder(Scene* scene)
        : m_worker(Worker::create())
    {
        connect(scene, &Scene::initScene, this, &PathFinder::generate);
        connect(this, &PathFinder::generated, scene, &Scene::initSceneSlot);
        connect(scene, &Scene::calcPath, this, &PathFinder::calculatePath);
        connect(this, &PathFinder::foundPath, scene, &Scene::foundPath);
    }

signals:
    void generated(const Field*);
    void foundPath(VectorHolder<int>*, int, int);

public slots:
    void calculatePath(Cell* start, Cell* goal) {
        if (!start || !goal) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!start || !goal";
            return ;
        }

        auto const start_idx = start->placeNum();
        auto const goal_idx = goal->placeNum();
        auto path = m_worker->findPath(start_idx, goal_idx);

//        QThread::msleep(5000);

        if (path.empty() || path[goal_idx] < 0) {

//            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "path hasn't been found";

            // not found
            emit foundPath(nullptr, 0, 0);
            return ;
        }

        auto wrappedPath = new VectorHolder<int>(std::move(path));
        emit foundPath(wrappedPath, int(start_idx), int(goal_idx));
    }

    void generate(int cols, int rows) {
        if (!m_worker) {
            m_worker = Worker::create();
        }

        if (!m_worker) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!worker";
            return ;
        }

        if (!m_worker->generateField(cols, rows)) {
            return;
        }

        auto field = m_worker->field();

        emit generated(field);
    }

private:
    std::unique_ptr<Worker> m_worker{};

}; // class


#endif // PATHFINDER_H
