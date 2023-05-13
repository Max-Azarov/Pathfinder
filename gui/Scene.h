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
    void setAccessedGenerate(bool);

    static int constexpr cellSize = 15;

protected:
    void wheelEvent(QGraphicsSceneWheelEvent* event) override;

private:
    bool m_isFirstPaint = true;

    Manager m_manager{this};
};
// ====================================================================================================================

#endif // SCENE_H
