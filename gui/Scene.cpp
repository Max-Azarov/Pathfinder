#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QMouseEvent>


#include "core/include/Field.h"
#include "gui/Scene.h"



// ====================================================================================================================
bool Scene::init(int cols, int rows) noexcept
{
    m_manager.clear();
    this->clear();
    if(!m_manager.init(cols, rows)) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!m_manager.init()";
        return false;
    }
//    initView();
    views().at(0)->setSceneRect(this->itemsBoundingRect());



//    clearLine();
//    m_cols = cols;
//    m_rows = rows;
//    m_cells.clear();

//    this->clear();
//    this->invalidate();

//    initThread();

//    emit initScene(cols, rows);

    return true;
}
// ====================================================================================================================
void Scene::initView() noexcept
{
    if (m_isFirstPaint && !views().isEmpty()) {
        views().at(0)->fitInView(sceneRect(), Qt::KeepAspectRatio);
        m_isFirstPaint = false;
    }
}
// ====================================================================================================================
void Scene::resizeView(const QSize& oldSize, const QSize& size) noexcept
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
// ====================================================================================================================
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
// ====================================================================================================================

