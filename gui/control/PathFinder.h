#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <memory>
#include <vector>

#include <QObject>
#include <QThread>
#include <QGraphicsScene>
#include <QDebug>

#include "core/include/Worker.h"
#include "gui/sceneItems/AbstractCell.h"
#include "gui/control/Manager.h"
#include "gui/control/VectorHolder.h"




class PathFinderThread : public QThread {
public:
    void run() override {
        this->exec();
    }
}; // class


class PathFinder : public QObject
{
    Q_OBJECT
public:
    PathFinder(Manager* manager)
        : m_worker(Worker::create())
    {
        connect(manager, &Manager::initScene, this, &PathFinder::generate);
        connect(manager, &Manager::calcPath, this, &PathFinder::calculatePath);
    }

signals:
    void generated(const Field*);
    void foundPath(VectorHolder<int>*, int, int);

public slots:
    void calculatePath(AbstractCell* start, AbstractCell* goal) {
        if (!start || !goal) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!start || !goal";
            return ;
        }

        auto const start_idx = start->placeNum;
        auto const goal_idx = goal->placeNum;
        auto path = m_worker->findPath(start_idx, goal_idx);

//        QThread::msleep(5000);

        if (path.empty() || path[goal_idx] < 0) {

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
