#ifndef SCENE_H
#define SCENE_H

#include <memory>
#include <array>
#include <vector>

#include <QGraphicsScene>

#include "core/include/Worker.h"
#include "gui/Cells.h"
#include "gui/VectorHolder.h"


class QGraphicsSceneWheelEvent;
class QThread;
class QMouseEvent;

class Field;
class PathFinder;


// ====================================================================================================================
class Scene : public QGraphicsScene
{
    Q_OBJECT

signals:
    void setEnabledGenerate(bool);
    void initScene(int, int);
    void calcPath(Cell*, Cell*);

public slots:
    void initSceneSlot(const Field*);
    void foundPath(VectorHolder<int>* wrappedPath, int start, int goal);
    void moveMouseOnItem(QGraphicsItem*);

public:
    Scene() = default;
    ~Scene();

    bool init(int cols, int rows) noexcept;
    void initView();
    void resizeView(const QSize& oldSize, const QSize& size);
    bool findPath(Cell* start, Cell* goal);

    void setA(Cell*);
    Cell* a() const;

    void setB(Cell*);
    Cell* b() const;

    void clearLine();

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    void createLine(int goal_idx, Qt::GlobalColor color = Qt::blue);
    void deleteChildItems(QGraphicsItem* item);
    void initThread();
    void destroyThread();

private:
    static int constexpr m_cellSize = 15;
    int m_rows{};
    int m_cols{};

    enum { A, B };
    std::array<Cell*, 2> m_ends{};

    bool m_isFirstPaint = true;

    std::vector<Cell*> m_cells{};

    QGraphicsLineItem* m_line;

    QThread* m_pathThread;
    PathFinder* m_pathFinder;

    QGraphicsItem* m_prevItem{};
};
// ====================================================================================================================

#endif // SCENE_H
