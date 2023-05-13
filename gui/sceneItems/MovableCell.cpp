#include <QPen>

#include "gui/sceneItems/MovableCell.h"
#include "gui/Scene.h"


// --------------------------------------------------------------------------------------------------------------------
MovableCell::MovableCell(Manager* manager, int num)
    : m_manager(manager)
{
    placeNum = num;
    this->setPen(QPen(Qt::darkGray));
}
// --------------------------------------------------------------------------------------------------------------------
 int MovableCell::type() const
 { return UserType + Manager::MOVABLE_CELL; }
 // --------------------------------------------------------------------------------------------------------------------
 void MovableCell::mousePressEvent(QGraphicsSceneMouseEvent*)
 {
     m_manager->onClicked(this);
 //    Scene* pScene = nullptr;
 //    try {
 //        pScene = dynamic_cast<Scene*>(this->scene());
 //    }
 //    catch(...) {
 //        return;
 //    }

 //    auto&& childs = this->childItems();

 //    // set / unset A, B
 //    if (childs.isEmpty()) {
 //        if(!pScene->a())  {
 //            pScene->setA(this);
 //            new ATextCell(this);
 //        }
 //        else if(!pScene->b())  {
 //            pScene->setB(this);
 //            new BTextCell(this);

 //            // finding
 //            auto const isPathExist = pScene->findPath(pScene->a(), this);
 //            if (!isPathExist) {
 //                qDebug() << __FILE__ << ":" << __LINE__ << ":" << "!isPathExist";
 //            }
 //        }
 //    }
 //    else {
 //        for (auto child : childs) {
 //            auto textCell = (QGraphicsSimpleTextItem*)child;
 //            if (textCell->text() == QString("B")) {
 //                pScene->setB(nullptr);
 //                delete child;
 //                childs.clear();

 //                pScene->clearLine();
 //            }
 //            else if (textCell->text() == QString("A") && !pScene->b()) {
 //                pScene->setA(nullptr);
 //                delete child;
 //                childs.clear();
 //            }
 //        }
 //    }
 }
// --------------------------------------------------------------------------------------------------------------------
