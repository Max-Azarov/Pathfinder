


#include "gui/Manager.h"
#include "gui/Scene.h"


Manager::Manager(Scene* scene)
{

}
// ====================================================================================================================
void Manager::initThread()
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
void Manager::destroyThread()
{
    m_pathThread->quit();
    m_pathThread->wait();
    delete m_pathThread;
    delete m_pathFinder;
}
// ====================================================================================================================
void Manager::initCells(const Field* field)
{
    if (!field) {
        return;
    }

    try {
        m_cells.resize(field->cellsNum());
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
// ====================================================================================================================

void Manager::foundPath(VectorHolder<int>* wrappedPath, int start, int goal)
{
    if (!wrappedPath || wrappedPath->value.size() != m_cells.size()) {
        //        qDebug() << __FILE__ << ":" << __LINE__ << ":" << "path hasn't been found";
        this->clearLine();
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

// ====================================================================================================================
