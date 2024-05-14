#include "myvideolist.h"

MyVideoList::MyVideoList(QWidget * parent )
{
    this->setParent(parent);
    hheader=this->horizontalHeader();hheader->setHidden(false);
    vheader=this->verticalHeader();vheader->setHidden(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setColumnCount(1);this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setFocusPolicy(Qt::NoFocus);this->setAcceptDrops(true);
    this->horizontalHeader()->setDefaultSectionSize(150);this->setColumnWidth(0,400);
    this->setDragDropMode(QAbstractItemView::DragDrop);this->setDragEnabled(true);
    this->viewport()->installEventFilter(parent);
    QStringList header;header<<tr("名称");this->setHorizontalHeaderLabels(header);

}
