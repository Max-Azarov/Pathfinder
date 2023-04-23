#include "gui/Scene.h"


void Cell::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    auto pScene = dynamic_cast<Scene*>(scene());

    if (childItems().isEmpty()) {
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << rect();

        if(!pScene->setA()) {
            new ATextCell(this);
        }
        else if(!pScene->setB()) {
            new BTextCell(this);
        }
    }
    else {
        auto childs = childItems();
        for (auto child : childs) {
            auto textCell = (QGraphicsSimpleTextItem*)child;
            if (textCell->text() == QString("B")) {
                pScene->unsetB();
                delete child;
            }
            else if (textCell->text() == QString("A") && !pScene->isBSet()) {
                pScene->unsetA();
                delete child;
            }
        }
        childs.clear();
        qDebug() << __FILE__ << ":" << __LINE__ << ":" << rect();
    }
}
