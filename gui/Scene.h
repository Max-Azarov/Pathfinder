#ifndef SCENE_H
#define SCENE_H


#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneWheelEvent>


class Scene : public QGraphicsScene
{
public:
    Scene() = default;

    QSize init(int cols, int rows) {
        auto const cellsNum = cols * rows;
        for (auto const& cell : cells) {
            removeItem(cell);
        }
        cells.clear();
        cells.reserve(cellsNum);
        m_cols = cols;
        m_rows = rows;

        for (int row = 0; row < m_rows; row++) {
            for (int col = 0; col < m_cols; col++) {
                auto cell = new QGraphicsRectItem();
                cell->setRect(col * m_cellSize, row * m_cellSize, m_cellSize, m_cellSize);
                addItem(cell);
                cells.push_back(cell);
            }
        }

        setSceneRect(0, 0,  m_cellSize * m_cols, m_cellSize * m_rows);

        isFirstPaint = true;

        return m_cellSize * QSize{m_cols, m_rows};
    }

    void initView()
    {
        if (isFirstPaint && !views().isEmpty()) {
            views().at(0)->fitInView(sceneRect(), Qt::KeepAspectRatio);
            isFirstPaint = false;
        }
    }


    void resizeView(const QSize& oldSize, const QSize& size)
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

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override
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

private:
    int m_rows{};
    int m_cols{};
    static int constexpr m_cellSize = 15;

    QVector<QGraphicsRectItem*> cells{};

    bool isFirstPaint = true;
};

#endif // SCENE_H
