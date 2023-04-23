#ifndef SCENE_H
#define SCENE_H


#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>



class ATextCell : public QGraphicsSimpleTextItem
{
public:
    ATextCell(QGraphicsRectItem* parent = nullptr)
        : QGraphicsSimpleTextItem(parent)
    {
        setFont(QFont("Arial", 12));
        setText("A");
        setPos(parent->rect().center() - boundingRect().center());
        setParentItem(parent);
    }
}; // class


class BTextCell : public QGraphicsSimpleTextItem
{
public:
    BTextCell(QGraphicsRectItem* parent = nullptr)
        : QGraphicsSimpleTextItem(parent)
    {
        setFont(QFont("Arial", 12));
        setText("B");
        setPos(parent->rect().center() - boundingRect().center());
        setParentItem(parent);
    }
}; // class


class Cell : public QGraphicsRectItem
{
public:
    Cell(QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(parent)
    {}

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

}; // class



class Scene : public QGraphicsScene
{
public:
    Scene() = default;

    QSize init(int cols, int rows)
    {
        this->clear();
        //        cells.clear();

        //        auto const cellsNum = cols * rows;
        //        cells.reserve(cellsNum);
        m_cols = cols;
        m_rows = rows;

        for (int row = 0; row < m_rows; row++) {
            for (int col = 0; col < m_cols; col++) {
                auto cell = new Cell();
                cell->setRect(col * m_cellSize, row * m_cellSize, m_cellSize, m_cellSize);
                addItem(cell);
                //                cells.push_back(cell);
            }
        }

        setSceneRect(0, 0,  m_cellSize * m_cols, m_cellSize * m_rows);

        m_isFirstPaint = true;
        unsetA();
        unsetB();

        return m_cellSize * QSize{m_cols, m_rows};
    }

    void initView()
    {
        if (m_isFirstPaint && !views().isEmpty()) {
            views().at(0)->fitInView(sceneRect(), Qt::KeepAspectRatio);
            m_isFirstPaint = false;
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

    bool setA() { auto const res = m_isACellSet; m_isACellSet = true; return res; }
    bool unsetA() { auto const res = m_isACellSet; m_isACellSet = false; return res; }
    bool isASet() { return m_isACellSet; }

    bool setB() { auto const res = m_isBCellSet; m_isBCellSet = true; return res; }
    bool unsetB() { auto const res = m_isBCellSet; m_isBCellSet = false; return res; }
    bool isBSet() { return m_isBCellSet; }

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

    //    QVector<QGraphicsRectItem*> cells{};

    bool m_isFirstPaint = true;
    bool m_isACellSet = false;
    bool m_isBCellSet = false;
};

#endif // SCENE_H
