#include "vitem.h"

//m_lineItem[c] = new QGraphicsLineItem();    // 定义一个直线图元
//m_lineItem[c]->setLine(QLineF(0, 0, 65, 0));
//m_lineItem[c]->setPen(tp);
//                m_lineItem->setFlag(QGraphicsItem::ItemIsMovable);
//tl->addItem(m_lineItem[c]);
//QPointF resp(20+100*c,15);m_lineItem[c]->setPos(resp);

vItem::vItem(int s,int e,int c,QString f, QGraphicsScene * sc,QPen p)
{
    start=s;end=e;filename=f;scene=sc;
//    this->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsrItem::ItemIsFocusable|QGraphicsItem::ItemIsSelectable |QGraphicsItem::ItemSendsGeometryChanges);
    this->setFlags(QGraphicsItem::ItemIsSelectable);
    this->setLine(QLineF(0,0,(e-s)/5000,0));
    this->setPen(p);
    sc->addItem(this);
    qDebug()<<(e-s)/5000;
    QPointF resp(c/5000+15,15);this->setPos(resp);

}
vItem::vItem(QGraphicsScene * sc)
{
    scene=sc;
    this->setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsFocusable|QGraphicsItem::ItemIsSelectable |QGraphicsItem::ItemSendsGeometryChanges);
    this->setLine(QLineF(0,0,0,40));
    QPen cP;QColor cC;cC.setRed(255);cP.setColor(cC);cP.setWidth(2);this->setPen(cP);
    sc->addItem(this);
    QPointF cresp(20,0);this->setPos(cresp);
}
