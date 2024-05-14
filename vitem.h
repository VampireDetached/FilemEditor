#ifndef VITEM_H
#define VITEM_H

#include <QGraphicsLineItem>
#include <QObject>
#include "QGraphicsScene"
#include "qgraphicsitem.h"
#include "QString"
#include "QPen"
#include "QLineF"

class vItem : public QGraphicsLineItem
{
public:
    vItem(int s,int e,int c,QString f, QGraphicsScene * sc,QPen p);
    vItem(QGraphicsScene * sc);

    int start=0;
    int end=0;
    QString filename;
    QGraphicsScene * scene;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value)
    {
        if (change == ItemPositionChange  &&  scene) // 控件发生移动
        {
            QPointF newPos = value.toPointF(); //即将要移动的位置
            QRectF rect(20,15,680,0); // 你要限制的区域
            if (!rect.contains(newPos)) // 是否在区域内
            {
                newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
                newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));
                return newPos;
            }
        }
        return QGraphicsItem::itemChange(change, value);
    };

};

#endif // VITEM_H
