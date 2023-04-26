#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>


#include "core/include/Field.h"
#include "gui/Scene.h"
#include "gui/PathFinder.h"


// ====================================================================================================================
Scene::~Scene()
{
    destroyThread();
}
// ====================================================================================================================
void Scene::initThread()
{
    if (m_pathThread) {
        return;
    }

    m_pathFinder = new PathFinder(this);
    m_pathThread = new PathFinderThread();
    m_pathFinder->moveToThread(m_pathThread);
    m_pathThread->start();
}
// ====================================================================================================================
void Scene::destroyThread()
{
    m_pathThread->quit();
    m_pathThread->wait();
    delete m_pathThread;
    delete m_pathFinder;
}
// ====================================================================================================================
bool Scene::init(int cols, int rows) noexcept
{
    this->clear();
    m_cols = cols;
    m_rows = rows;
    m_line = nullptr;

    initThread();

    emit initScene(cols, rows);

    return true;
}


void Scene::initSceneSlot(Field* field)
{
    if (!field) {
        return;
    }

    try {
        this->m_cells.resize(field->cellsNum());
    }
    catch(...) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.second != cols";
        return ;
    }

    Cell* cell{};
    int placeNum = 0;
    for (int row = 0; row < m_rows; row++) {
        auto const& cur_row = field->row(row);
        if (cur_row.second != m_cols) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.second != cols";
            return ;
        }
        for (int col = 0; col < m_cols; col++) {
            if (cur_row.first[col] == Field::ECellType::WALL) {
                auto pCell = new WallCell();
                pCell->setPlaceNum(placeNum);
                pCell->setBrush(QBrush(Qt::red));
                pCell->setPen(QPen(Qt::darkGray));
                cell = pCell;
            }
            else {
                auto pCell = new MovableCell();
                pCell->setPen(QPen(Qt::darkGray));
                pCell->setPlaceNum(placeNum);
                cell = pCell;
            }
            cell->setRect(col * m_cellSize, row * m_cellSize, m_cellSize, m_cellSize);

            this->addItem(cell);
            m_cells[placeNum] = cell;

            placeNum++;
        }
    }

    this->setSceneRect(0, 0,  m_cellSize * m_cols, m_cellSize * m_rows);

    this->m_isFirstPaint = true;
    this->setA(nullptr);
    this->setB(nullptr);

    // Обновляем сцену с результатами вычислений
    this->update();
}


void Scene::initView()
{
    if (m_isFirstPaint && !views().isEmpty()) {
        views().at(0)->fitInView(sceneRect(), Qt::KeepAspectRatio);
        m_isFirstPaint = false;
    }
}


void Scene::resizeView(const QSize& oldSize, const QSize& size)
{
    if (views().isEmpty() ||
        oldSize.isEmpty() ||
        size.isEmpty()) {
        return;
    }

    if (oldSize.width() < size.width() || oldSize.height() < size.height()) {
        auto const scaleFactor = qMax(double(size.width()) / oldSize.width(), double(size.height()) / oldSize.height());
        views().at(0)->scale(scaleFactor, scaleFactor);
    }

    auto const& view = views().at(0);
    auto const maxScale = qMin(view->rect().width() / sceneRect().width(), view->rect().height() / sceneRect().height());
    auto const currentScale = views().at(0)->transform().m11();

    if (currentScale > maxScale) {
        view->fitInView(sceneRect(), Qt::KeepAspectRatio);
        return;
    }
}


bool Scene::findPath(Cell* start, Cell* goal)
{
    emit setEnabledGenerate(false);
    emit calcPath(start, goal);

    return true;
}


void Scene::foundPath(VectorHolder<int>* wrappedPath, int start, int goal)
{
    if (!wrappedPath || wrappedPath->value.size() != m_cells.size()) {
//        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "path hasn't been found";
        emit setEnabledGenerate(true);
        return;
    }

    auto idx = goal;
    while(idx != start) {
        auto const nextIdx = wrappedPath->value[idx];
        m_cells[idx]->setPathCell(m_cells[nextIdx]);
        idx = nextIdx;
    }
    m_cells[start]->setPathCell(nullptr);

    this->createLine(goal);

    this->update();

    delete wrappedPath;

    emit setEnabledGenerate(true);
}


void Scene::setA(Cell* cell) { m_ends[A] = cell; }
Cell* Scene::a() const { return m_ends[A]; }

void Scene::setB(Cell* cell) { m_ends[B] = cell; }
Cell* Scene::b() const { return m_ends[B]; }


void Scene::wheelEvent(QGraphicsSceneWheelEvent* event)
{
    if (views().isEmpty()) {
        return;
    }

    auto constexpr scaleUp = 1.1;
    auto constexpr scaleDown = 0.9;

    auto scaleFactor = event->delta() > 0 ? scaleUp : scaleDown;

    auto const& view = views().at(0);

    auto const maxScale = qMin(view->rect().width() / sceneRect().width(), view->rect().height() / sceneRect().height());
    auto const minScale = 1.0;

    // Получаем текущий масштаб
    auto const currentScale = view->transform().m11();

    auto const destScale = currentScale * scaleFactor;
    if (destScale > maxScale || destScale < scaleDown - 0.1) {
        view->fitInView(sceneRect(), Qt::KeepAspectRatio);
        return;
    }
    else if (destScale < minScale) {
        scaleFactor = minScale / currentScale;
    }

    views().at(0)->scale(scaleFactor, scaleFactor);
}


void Scene::createLine(int goal_idx)
{
    Cell* cell1 = m_cells[goal_idx];
    if (!cell1) {
        return;
    }

    clearLine();

    QGraphicsLineItem* prevLine = nullptr;
    Cell* cell2 = cell1->pathCell();

    while (cell2 != nullptr) {
        auto line = new QGraphicsLineItem();
        auto const p1 = cell1->rect().center();
        auto const p2 = cell2->rect().center();
        line->setLine(p1.x(), p1.y(), p2.x(), p2.y());
        line->setPen(QPen(Qt::blue));
        if (!m_line) {
            m_line = line;
        }
        line->setParentItem(prevLine);

        prevLine = line;

        cell1 = cell2;
        cell2 = cell2->pathCell();
    }

    if (m_line) {
        this->addItem(m_line);
    }
}


void Scene::deleteChildItems(QGraphicsItem* item) {
    if (!item) {
        return;
    }
    else {
        for(auto childItem : item->childItems()) {
            deleteChildItems(childItem);
        }
        delete item;
    }
}


void Scene::clearLine()
{
    if (m_line) {
        this->removeItem(m_line);
        deleteChildItems(m_line);
        m_line = nullptr;
    }
}

