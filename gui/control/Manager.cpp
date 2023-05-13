

#include "gui/control/Manager.h"
#include "gui/control/FieldCreatorAndPathFinder.h"
#include "gui/control/VectorHolder.h"
#include "gui/Scene.h"
#include "gui/sceneItems/PathLine.h"
#include "gui/sceneItems/AbstractCell.h"
#include "gui/sceneItems/AbstractWayPoint.h"
#include "gui/sceneItems/WallCell.h"
#include "gui/sceneItems/MovableCell.h"
#include "gui/sceneItems/AWayPoint.h"
#include "gui/sceneItems/BWayPoint.h"
#include "gui/MyGraphicsView.h"


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

        if (!m_scene->views().empty()) {
            auto view = (MyGraphicsView*)m_scene->views().front();
            QObject::connect(view, &MyGraphicsView::hoverItem, this, &Manager::moveMouseOnItem);
        }
    }
    catch(...) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "catch(...)";
        return ;
    }
}
// ====================================================================================================================
void Manager::foundPath(VectorHolder<int>* wrappedPath, int start, int goal)
{
    if (!wrappedPath || wrappedPath->value.size() != m_field.size()) {
        removePathLine();
        m_scene->setAccessedGenerate(true);
        return;
    }

    try {
        m_pathLine = std::make_unique<PathLine>(this);
    }
    catch(...) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "catch(...)";
        return;
    }

    {
        // calc line length
        auto length = 1;
        auto idx = goal;
        while(idx != start) {
            auto const nextIdx = wrappedPath->value[idx];
            idx = nextIdx;
            length++;
        }
        m_pathLine->reserve(length);
    }
    {
        // create the path line by adding the lines
        auto idx = goal;
        while(idx != start) {
            auto const nextIdx = wrappedPath->value[idx];
            auto const p1 = m_field[idx]->rect().center();
            auto const p2 = m_field[nextIdx]->rect().center();
            m_pathLine->add(QLineF{p1, p2});
            idx = nextIdx;
        }
    }

    for(auto const& item : m_pathLine->items()) {
        m_scene->addItem(item.get());
    }

    m_scene->update();

    delete wrappedPath;

    m_scene->setAccessedGenerate(true);
}
// ====================================================================================================================
void Manager::clear() noexcept
{
    m_wayPoints.clear();
    removePathLine();
    m_field.clear();

    m_scene->clear();
}
// ====================================================================================================================
void Manager::onClicked(AbstractCell* item) noexcept
{
    if (!item) {
        return;
    }

    enum {
        NONE, ONLY_A, AB
    };

    try {
        if (m_wayPoints.empty()) {
            // add A
            auto cell = std::make_unique<AWayPoint>(this);
            cell->setPos(item->rect().center() - cell->boundingRect().center());
            cell->setParentItem(item);
            m_wayPoints.emplace_back(std::move(cell));
        }
        else if (m_wayPoints.size() == ONLY_A) {
            auto&& childs = item->childItems();

            if (childs.isEmpty()) {
                // add B
                auto cell = std::make_unique<BWayPoint>(this);
                cell->setPos(item->rect().center() - cell->boundingRect().center());
                cell->setParentItem(item);
                m_wayPoints.emplace_back(std::move(cell));
            }
            else {
                // remove A
                childs.clear();
                m_wayPoints.clear();
            }
        }
        else if (m_wayPoints.size() == AB) {
            auto&& childs = item->childItems();

            if (!childs.isEmpty() && childs.front()->type() == BWayPoint(this).type()) {
                // remove B
                childs.clear();
                m_wayPoints.resize(ONLY_A);

                removePathLine();
            }
            else {
                return;
            }
        }

        if (m_wayPoints.size() == AB) {
            m_scene->setAccessedGenerate(false);
            emit calcPath((AbstractCell*)m_wayPoints[0]->parentItem(),
                    (AbstractCell*)m_wayPoints[1]->parentItem());
        }
    }
    catch(...) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "catch(...) {}";
    }

}
// ====================================================================================================================
void Manager::removePathLine() noexcept
{ m_pathLine.reset(); }
// ====================================================================================================================
void Manager::moveMouseOnItem(QGraphicsItem* item)
{
    enum {
        NONE, ONLY_A, AB
    };

    if (!item) {
        if (m_wayPoints.size() != AB) {
            removePathLine();
        }
        return;
    }

    if (item->type() == QGraphicsLineItem().type() || m_prevMoveMouseOnItem == item) {
        return;
    }

    m_prevMoveMouseOnItem = item;

    if (m_wayPoints.size() == ONLY_A) {
        removePathLine();
        if(item->type() == MovableCell(this, 0).type()) {
            MovableCell* pCell = nullptr;
            try {
                pCell = dynamic_cast<MovableCell*>(item);
            }
            catch(...) {
                qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!item";
                return;
            }

            auto&& childs = item->childItems();

            if (childs.isEmpty()) {
                m_scene->setAccessedGenerate(false);
                emit calcPath((AbstractCell*)m_wayPoints[0]->parentItem(), pCell);
            }
        }
    }
}
// ====================================================================================================================
