#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QMouseEvent>


#include "core/include/Field.h"
#include "gui/Scene.h"
#include "gui/PathFinder.h"


// ====================================================================================================================
bool Scene::init(int cols, int rows) noexcept
{
    clearLine();
    m_cols = cols;
    m_rows = rows;
    m_cells.clear();

    this->clear();
    this->invalidate();

    initThread();

    emit initScene(cols, rows);

    return true;
}

