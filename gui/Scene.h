#ifndef SCENE_H
#define SCENE_H


#include <QGraphicsScene>

#include "gui/Manager.h"


// ====================================================================================================================
class Scene : public QGraphicsScene
{
    Q_OBJECT
signals:
    void setEnabledGenerate(bool);


public:
    bool init(int cols, int rows) noexcept;
    void initView() noexcept;
    void resizeView(const QSize& oldSize, const QSize& size) noexcept;

private:
    static int constexpr m_cellSize = 15;
    int m_rows{};
    int m_cols{};

    bool m_isFirstPaint = true;

    Manager m_manager{this};
};
// ====================================================================================================================

#endif // SCENE_H
