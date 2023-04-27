#ifndef MYGRAPHICVIEW_H
#define MYGRAPHICVIEW_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QDebug>


class MyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    MyGraphicsView(QWidget* parent = nullptr)
        : QGraphicsView(parent)
    {}

signals:
    void hoverItem(QGraphicsItem*);

protected:
    void mouseMoveEvent(QMouseEvent* event) override
    {
        QGraphicsItem* item = itemAt(event->pos());
//        qDebug() << event->pos();
        emit hoverItem(item);

        QGraphicsView::mouseMoveEvent(event);
    }

};

#endif // MYGRAPHICVIEW_H
