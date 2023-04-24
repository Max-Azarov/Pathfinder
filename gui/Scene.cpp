#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>


#include "gui/Scene.h"
#include "core/include/Worker.h"


// ====================================================================================================================
TextCell::TextCell(QGraphicsItem* parent)
    : QGraphicsSimpleTextItem(parent)
{}
// ====================================================================================================================
ATextCell::ATextCell(QGraphicsRectItem* parent)
    : TextCell(parent)
{
    setFont(QFont("Arial", 12));
    setText("A");
    setPos(parent->rect().center() - boundingRect().center());
    setParentItem(parent);
}
// ====================================================================================================================
BTextCell::BTextCell(QGraphicsRectItem* parent)
    : TextCell(parent)
{
    setFont(QFont("Arial", 12));
    setText("B");
    setPos(parent->rect().center() - boundingRect().center());
    setParentItem(parent);
}
// ====================================================================================================================
Cell::Cell(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{}

void Cell::setPlaceNum(int placeNum) { m_placeNum = placeNum; }
int Cell::placeNum() { return m_placeNum; }

void Cell::setPathCell(Cell* cell) { m_pathCell = cell; }
Cell* Cell::pathCell() { return m_pathCell; }
// ====================================================================================================================
WallCell::WallCell(QGraphicsItem* parent)
    : Cell(parent)
{}
// ====================================================================================================================
MovableCell::MovableCell(QGraphicsItem* parent)
    : Cell(parent)
{}

void MovableCell::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    Scene* pScene = nullptr;
    try {
        pScene = dynamic_cast<Scene*>(this->scene());
    }
    catch(...) {
        return;
    }

    auto&& childs = this->childItems();

    // set / unset A, B
    if (childs.isEmpty()) {
        if(!pScene->a())  {
            pScene->setA(this);
            new ATextCell(this);
        }
        else if(!pScene->b())  {
            pScene->setB(this);
            new BTextCell(this);

            // finding
            auto const isPathExist = pScene->findPath(pScene->a(), this);
            if (!isPathExist) {
                qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!isPathExist";
            }
        }
    }
    else {
        for (auto child : childs) {
            auto textCell = (QGraphicsSimpleTextItem*)child;
            if (textCell->text() == QString("B")) {
                pScene->setB(nullptr);
                delete child;
                childs.clear();

                pScene->clearLine();
            }
            else if (textCell->text() == QString("A") && !pScene->b()) {
                pScene->setA(nullptr);
                delete child;
                childs.clear();
            }
        }
    }
}
// ====================================================================================================================
bool Scene::init(int cols, int rows) noexcept
{
    this->clear();
    m_cols = cols;
    m_rows = rows;
    m_line = nullptr;

    if (!m_worker) {
        m_worker = Worker::create();
    }

    if (!m_worker) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!worker";
        return false;
    }

    if (!m_worker->generateField(cols, rows)) {
        return false;
    }

    auto const& field = m_worker->field();

    try {
        m_cells.resize(field.cellsNum());
    }
    catch(...) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.second != cols";
        return false;
    }

    Cell* cell{};
    int placeNum = 0;
    for (int row = 0; row < rows; row++) {
        auto const& cur_row = field.row(row);
        if (cur_row.second != cols) {
            qDebug() << __FILE__ << ":" << __LINE__ << ":" << "cur_row.second != cols";
            return false;
        }
        for (int col = 0; col < cols; col++) {
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

            addItem(cell);
            m_cells[placeNum] = cell;

            placeNum++;
        }
    }

    setSceneRect(0, 0,  m_cellSize * cols, m_cellSize * rows);

    m_isFirstPaint = true;
    setA(nullptr);
    setB(nullptr);

    return true;
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
    if (!start || !goal) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!start || !goal";
        return false;
    }

    auto const start_idx = start->placeNum();
    auto const goal_idx = goal->placeNum();
    auto path = m_worker->findPath(start_idx, goal_idx);

    if (path.empty() || path[goal_idx] < 0) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "path hasn't been found";
        return false;
    }

    for (auto& cell : m_cells) {
        cell->setPathCell(nullptr);
    }

    auto idx = goal_idx;
    while(idx != start_idx) {
        auto const nextIdx = path[idx];
        m_cells[idx]->setPathCell(m_cells[nextIdx]);
        idx = nextIdx;
    }

    this->createLine(goal_idx);

    return true;
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


void Scene::clearLine()
{
    if (m_line) {
        this->removeItem(m_line);
        m_line = nullptr;
    }
}

