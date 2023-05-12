

#include "gui/control/Manager.h"
#include "gui/control/PathFinder.h"
#include "gui/control/VectorHolder.h"
#include "gui/Scene.h"
#include "gui/sceneItems/PathLine.h"
#include "gui/sceneItems/AbstractCell.h"
#include "gui/sceneItems/AbstractWayPoint.h"
#include "gui/sceneItems/WallCell.h"
#include "gui/sceneItems/MovableCell.h"
#include "gui/sceneItems/AWayPoint.h"
#include "gui/sceneItems/BWayPoint.h"


Manager::Manager(Scene* scene)
    :m_scene(scene)
{

}
// ====================================================================================================================
Manager::~Manager()
{
    this->destroyThread();
}
// ====================================================================================================================
bool Manager::init(int cols, int rows) noexcept
{
    if (!m_scene) {
        return false;
    }

    try {
        this->initThread();

        emit initScene(cols, rows);

        return true;
    }
    catch(...) {}

    return false;
}
// ====================================================================================================================
void Manager::initThread()
{
    if (m_pathThread) {
        return;
    }

    m_pathFinder = std::make_unique<PathFinder>(this);
    m_pathThread = std::make_unique<PathFinderThread>();
    m_pathFinder->moveToThread(m_pathThread.get());

    connect(m_pathFinder.get(), &PathFinder::generated, this, &Manager::initCells);
    connect(m_pathFinder.get(), &PathFinder::foundPath, this, &Manager::foundPath);

    m_pathThread->start();
}
// ====================================================================================================================
void Manager::destroyThread() noexcept
{
    m_pathThread->quit();
    m_pathThread->wait();
}
// ====================================================================================================================
void Manager::initCells(const Field* field)
{
    if (!field || !field->cellsNum()) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!field || !field->cellsNum()";
        return;
    }

    auto const cellsNum = field->cellsNum();

    try {
        auto const cols = field->row(0).second;
        auto const rows = cellsNum / cols;
        if (cellsNum % cols) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cellsNum % cols";
            return;
        }

        m_field.resize(cellsNum);
        int placeNum = 0;
        auto cell = std::unique_ptr<AbstractCell>();
        for (int row = 0; row < rows; row++) {
            auto const& cur_row = field->row(row);
            if (cur_row.second != cols) {
                qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.second != cols";
                return ;
            }
            for (int col = 0; col < cols; col++) {
                switch (cur_row.first[col]) {
                case Field::ECellType::WALL:
                    cell = std::make_unique<WallCell>(this, placeNum);
                    break;
                case Field::ECellType::MOVABLE:
                    cell = std::make_unique<MovableCell>(this, placeNum);
                    break;
                default:
                    qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.first[col] == default";
                    return;
                }
                cell->setRect(col * Scene::cellSize, row * Scene::cellSize, Scene::cellSize, Scene::cellSize);

                m_scene->addItem(cell.get());
                m_field[placeNum] = std::move(cell);

                placeNum++;
                cell.reset();
            }
        }

        m_scene->setSceneRect(0, 0,  Scene::cellSize * cols, Scene::cellSize * rows);
        m_scene->initView();
        m_scene->update();
    }
    catch(...) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.second != cols";
        return ;
    }
}
// ====================================================================================================================

void Manager::foundPath(VectorHolder<int>* wrappedPath, int start, int goal)
{
    //    if (!wrappedPath || wrappedPath->value.size() != m_field.size()) {
    //        //        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "path hasn't been found";
    //        this->clearLine();
    //        emit setEnabledGenerate(true);
    //        return;
    //    }

    //    auto idx = goal;
    //    while(idx != start) {
    //        auto const nextIdx = wrappedPath->value[idx];
    //        m_cells[idx]->setPathCell(m_cells[nextIdx]);
    //        idx = nextIdx;
    //    }
    //    m_cells[start]->setPathCell(nullptr);

    //    this->createLine(goal);

    //    this->update();

    //    delete wrappedPath;

    //    emit setEnabledGenerate(true);
}

// ====================================================================================================================
void Manager::clear() noexcept
{
    m_state.common = 0;

    // TODO remove PathLine from scene
    m_pathLine.reset();

    // TODO remove AbstractCell from scene
    m_field.clear();

    // TODO remove AbstractWayPoint from scene
    m_wayPoints.clear();
}
// ====================================================================================================================
