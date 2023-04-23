#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>


#include "gui/Scene.h"
#include "core/include/Worker.h"


// ====================================================================================================================
ATextCell::ATextCell(QGraphicsRectItem* parent)
    : QGraphicsSimpleTextItem(parent)
{
    setFont(QFont("Arial", 12));
    setText("A");
    setPos(parent->rect().center() - boundingRect().center());
    setParentItem(parent);
}
// ====================================================================================================================
BTextCell::BTextCell(QGraphicsRectItem* parent)
    : QGraphicsSimpleTextItem(parent)
{
    setFont(QFont("Arial", 12));
    setText("B");
    setPos(parent->rect().center() - boundingRect().center());
    setParentItem(parent);
}
// ====================================================================================================================
WallCell::WallCell(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{}
// ====================================================================================================================
Cell::Cell(QGraphicsItem* parent)
    : QGraphicsRectItem(parent)
{}

void Cell::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    auto pScene = dynamic_cast<Scene*>(scene());
    auto&& childs = childItems();

    if (childs.isEmpty()) {
        if(!pScene->setA()) {
            new ATextCell(this);
        }
        else if(!pScene->setB()) {
            // finding


            new BTextCell(this);
        }
    }
    else {
        for (auto child : childs) {
            auto textCell = (QGraphicsSimpleTextItem*)child;
            if (textCell->text() == QString("B")) {
                pScene->unsetB();
                delete child;
                childs.clear();
            }
            else if (textCell->text() == QString("A") && !pScene->isBSet()) {
                pScene->unsetA();
                delete child;
                childs.clear();
            }
        }
    }
}
// ====================================================================================================================
QSize Scene::init(int cols, int rows)
{
    clear();
    m_cols = cols;
    m_rows = rows;

    if (!m_worker) {
        m_worker = Worker::create();
    }

    if (!m_worker) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!worker";
        return QSize{};
    }

    if (!m_worker->generateField(cols, rows)) {
        return QSize{};
    }

    auto const& field = m_worker->field();
    QGraphicsRectItem* cell{};
    for (int row = 0; row < rows; row++) {
        auto const& cur_row = field.row(row);
        if (cur_row.second != cols) {
            return QSize{};
        }
        for (int col = 0; col < cols; col++) {
            if (cur_row.first[col] == Field::ECellType::WALL) {
                cell = new WallCell();
                cell->setBrush(QBrush(QColor(255, 0, 0)));
            }
            else {
                cell = new Cell();
            }
            cell->setRect(col * m_cellSize, row * m_cellSize, m_cellSize, m_cellSize);
            addItem(cell);
        }
    }

    setSceneRect(0, 0,  m_cellSize * cols, m_cellSize * rows);

    m_isFirstPaint = true;
    unsetA();
    unsetB();

    return m_cellSize * QSize{cols, rows};
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

bool Scene::setA() { auto const res = m_isACellSet; m_isACellSet = true; return res; }
bool Scene::unsetA() { auto const res = m_isACellSet; m_isACellSet = false; return res; }
bool Scene::isASet() { return m_isACellSet; }

bool Scene::setB() { auto const res = m_isBCellSet; m_isBCellSet = true; return res; }
bool Scene::unsetB() { auto const res = m_isBCellSet; m_isBCellSet = false; return res; }
bool Scene::isBSet() { return m_isBCellSet; }


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

