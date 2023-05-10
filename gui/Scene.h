#ifndef SCENE_H
#define SCENE_H


#include <QGraphicsScene>

#include "gui/Manager.h"


// ====================================================================================================================
class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    bool init(int cols, int rows) noexcept;

private:
    static int constexpr m_cellSize = 15;
    int m_rows{};
    int m_cols{};

    Manager m_manager{};
};
// ====================================================================================================================

#endif // SCENE_H
