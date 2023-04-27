#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QGraphicsView>


class MyGraphicsView : public QGraphicsView
{
public:
    MyGraphicsView(QWidget* parent = nullptr)
        : QGraphicsView(parent)
    {}

protected:
    void mouseMoveEvent(QMouseEvent* event) override
    {
        QGraphicsItem* item = itemAt(event->pos());
        if (item)
        {
            qDebug() << "Размеры QGraphicsItem:" << item->boundingRect().width() << "x" << item->boundingRect().height();
        }

        QGraphicsView::mouseMoveEvent(event);
    }

};

#endif // MYGRAPHICVIEW_H
