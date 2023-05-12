#ifndef SCENE_H
#define SCENE_H


#include <QGraphicsScene>

#include "gui/control/Manager.h"


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

    static int constexpr cellSize = 15;
private:
//    int m_rows{};
//    int m_cols{};

    bool m_isFirstPaint = true;

    Manager m_manager{this};
};
// ====================================================================================================================

#endif // SCENE_H
